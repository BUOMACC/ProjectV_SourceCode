// Fill out your copyright notice in the Description page of Project Settings.


#include "SitAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_NetworkSyncPoint.h"
#include "CommonActivatableWidget.h"
#include "Interface/CombatUILayerInterface.h"
#include "Interface/InteractionInterface.h"
#include "Player/CombatPlayerController.h"
#include "UI/CombatHUD.h"

USitAbility::USitAbility()
{
	AbilityTags.AddTag(FCombatGameplayTags::Get().Ability_Type_Action_InteractActive);
	AbilityTags.AddTag(FCombatGameplayTags::Get().ActivationType_Active);

	CancelAbilitiesWithTag.AddTag(FCombatGameplayTags::Get().Ability_Type_Action_Guard);
	CancelAbilitiesWithTag.AddTag(FCombatGameplayTags::Get().Ability_Type_Movement_Run);

	ActivationRequiredTags.AddTag(FCombatGameplayTags::Get().Character_MovementMode_Walking);

	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Input_State_DisableFirstCombo);
	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Character_State_Invincible);
	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Character_State_Interact);

	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;

	InputTag = FCombatGameplayTags::Get().Input_Type_Interact;
	bAutoTriggerWhenInput = true;

	SitMontage = nullptr;
	SitExitMontage = nullptr;
	PlayRate = 1.f;

	WidgetToOpen = nullptr;
	WidgetOpenDelay = 4.f;
	TimerHandle.Invalidate();

	WarpingName = TEXT("Sit");
}

void USitAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!IsValid(GetSourceObject(Handle, ActorInfo)))
	{
		UE_LOG(LogTemp, Error, TEXT("USitAbility::ActivateAbility - SourceObject is nullptr."), *GetName());
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if (SitMontage == nullptr || SitExitMontage == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("USitAbility::ActivateAbility - Montage is nullptr. [%s]"), *GetName());
		return;
	}

	// 상호작용 메시지 전송
	if (HasAuthority(&CurrentActivationInfo))
	{
		if (GetSourceObject(Handle, ActorInfo)->Implements<UInteractionInterface>())
		{
			IInteractionInterface::Execute_OnInteract(GetSourceObject(Handle, ActorInfo), GetOwningActorFromActorInfo(), GetAvatarActorFromActorInfo());
		}
	}

	UAbilityTask_NetworkSyncPoint* NetSyncTask = UAbilityTask_NetworkSyncPoint::WaitNetSync(this, EAbilityTaskNetSyncType::OnlyClientWait);
	NetSyncTask->OnSync.AddDynamic(this, &USitAbility::OnNetSync);
	NetSyncTask->ReadyForActivation();
}

bool USitAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void USitAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// 상호작용 종료 메시지 전송
	if (HasAuthority(&CurrentActivationInfo))
	{
		UObject* InteractableObject = GetSourceObject(Handle, ActorInfo);
		if (IsValid(InteractableObject) && InteractableObject->Implements<UInteractionInterface>())
		{
			IInteractionInterface::Execute_OnInteractFinished(GetSourceObject(Handle, ActorInfo), GetOwningActorFromActorInfo(), GetAvatarActorFromActorInfo());
		}
	}

	GetCombatCharacter()->GetMotionWarpingComponent()->RemoveWarpTarget(WarpingName);

	if (TimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void USitAbility::OnNetSync()
{
	const AActor* TargetActor = Cast<AActor>(GetSourceObject(CurrentSpecHandle, CurrentActorInfo));

	FVector WarpingLocation = TargetActor->GetActorLocation() + (TargetActor->GetActorForwardVector() * 20.f);
	FRotator WarpingRotation = TargetActor->GetActorRotation();

	GetCombatCharacter()->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocationAndRotation(WarpingName, WarpingLocation, WarpingRotation);

	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, SitMontage, PlayRate, NAME_None, true, 1.0f);
	Task->ReadyForActivation();

	// Reserve Add Widget
	if (IsLocallyControlled())
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::OnDelay, WidgetOpenDelay, false);
	}
}

void USitAbility::CallProcessExitSitting()
{
	ProcessExitSitting();

	if (IsPredictingClient())
	{
		ProcessExitSitting_Server();
	}
}

void USitAbility::ProcessExitSitting()
{
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, SitExitMontage, PlayRate, NAME_None, true, 1.0f);
	Task->OnInterrupted.AddDynamic(this, &USitAbility::OnSitExitMontageInterrupted);
	Task->OnCompleted.AddDynamic(this, &USitAbility::OnSitExitMontageCompleted);
	Task->OnCancelled.AddDynamic(this, &USitAbility::OnSitExitMontageCanceled);
	Task->ReadyForActivation();
}

void USitAbility::ProcessExitSitting_Server_Implementation()
{
	ProcessExitSitting();
}

void USitAbility::OnSitExitMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USitAbility::OnSitExitMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USitAbility::OnSitExitMontageCanceled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void USitAbility::OnDelay()
{
	if (WidgetToOpen == nullptr)
	{
		CallProcessExitSitting();
		return;
	}

	ACombatPlayerController* CombatPC = Cast<ACombatPlayerController>(GetActorController());
	if (CombatPC == nullptr)
	{
		CallProcessExitSitting();
		return;
	}

	ACombatHUD* CombatHUD = Cast<ACombatHUD>(CombatPC->GetHUD());
	if (CombatHUD == nullptr || !CombatHUD->Implements<UCombatUILayerInterface>())
	{
		CallProcessExitSitting();
		return;
	}

	UCommonActivatableWidget* Widget = ICombatUILayerInterface::Execute_AddWidget(CombatHUD, FCombatGameplayTags::Get().UI_Layer_Game, WidgetToOpen);
	if (Widget)
	{
		Widget->OnDeactivated().AddUObject(this, &ThisClass::CallProcessExitSitting);
	}
}

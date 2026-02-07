// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathBlowExecutorAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_NetworkSyncPoint.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "MotionWarpingComponent.h"
#include "Player/CombatPlayerController.h"

UDeathBlowExecutorAbility::UDeathBlowExecutorAbility()
{
	AbilityTags.AddTag(FCombatGameplayTags::Get().Ability_Type_Action_DeathBlow);
	AbilityTags.AddTag(FCombatGameplayTags::Get().ActivationType_Active);

	CancelAbilitiesWithTag.AddTag(FCombatGameplayTags::Get().Ability_Type_Action_Guard);
	CancelAbilitiesWithTag.AddTag(FCombatGameplayTags::Get().Ability_Type_Movement_Run);

	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Input_State_DisableFirstCombo);
	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Character_State_Invincible);
	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Character_State_DeathBlow);

	ActivationRequiredTags.AddTag(FCombatGameplayTags::Get().Character_MovementMode_Walking);

	// Default Values
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	WarpingName = TEXT("DeathBlow");
}

void UDeathBlowExecutorAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!IsValid(GetSourceObject(Handle, ActorInfo)))
	{
		UE_LOG(LogTemp, Error, TEXT("UDeathBlowExecutorAbility::ActivateAbility - Cancelled. SourceObject is nullptr."));
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	TargetActor = Cast<AActor>(GetSourceObject(Handle, ActorInfo));
	if (!TargetActor.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("UDeathBlowExecutorAbility::ActivateAbility - Cancelled. SourceObject is not Actor"));
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	if (DeathBlowMontage == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UDeathBlowExecutorAbility::ActivateAbility - Montage is nullptr. [%s]"), *GetName());
		return;
	}

	UAbilityTask_NetworkSyncPoint* NetSyncTask = UAbilityTask_NetworkSyncPoint::WaitNetSync(this, EAbilityTaskNetSyncType::OnlyClientWait);
	NetSyncTask->OnSync.AddDynamic(this, &UDeathBlowExecutorAbility::OnNetSync);
	NetSyncTask->ReadyForActivation();
}

void UDeathBlowExecutorAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (const AController* Controller = GetActorController())
	{
		if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{
			if (const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
			{
				if (UEnhancedInputLocalPlayerSubsystem* Inputsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
				{
					if (InputMappingContext)
					{
						FModifyContextOptions Options;
						Options.bForceImmediately = true;
						Inputsystem->AddMappingContext(InputMappingContext, 1, Options);
					}
				}
			}
		}
	}
}

void UDeathBlowExecutorAbility::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnRemoveAbility(ActorInfo, Spec);

	if (const AController* Controller = GetActorController())
	{
		if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{
			if (const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
			{
				if (UEnhancedInputLocalPlayerSubsystem* Inputsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
				{
					if (InputMappingContext)
					{
						FModifyContextOptions Options;
						Options.bForceImmediately = true;
						Inputsystem->RemoveMappingContext(InputMappingContext, Options);
					}
				}
			}
		}
	}
}

bool UDeathBlowExecutorAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UDeathBlowExecutorAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (TargetActor.IsValid())
	{
		TargetActor.Reset();
	}

	GetCombatCharacter()->GetMotionWarpingComponent()->RemoveWarpTarget(WarpingName);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDeathBlowExecutorAbility::OnNetSync()
{
	FVector WarpingLocation;
	FRotator WarpingRotation;
	FindBestLocationAndRotation(TargetActor.Get(), WarpingLocation, WarpingRotation);

	GetCombatCharacter()->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocationAndRotation(WarpingName, WarpingLocation, WarpingRotation);

	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, DeathBlowMontage, 1.f, NAME_None, true, 1.0f);
	MontageTask->OnCompleted.AddDynamic(this, &UDeathBlowExecutorAbility::OnMontageCompleted);
	MontageTask->OnInterrupted.AddDynamic(this, &UDeathBlowExecutorAbility::OnMontageInterrupted);
	MontageTask->OnCancelled.AddDynamic(this, &UDeathBlowExecutorAbility::OnMontageCanceled);
	MontageTask->ReadyForActivation();

	// 타겟이 맞는 어빌리티를 실행하도록 이벤트 전송
	FGameplayEventData Payload;
	Payload.Instigator = GetCombatCharacter();
	Payload.ContextHandle = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetActor.Get(), DeathBlowTargetTag, Payload);
}

void UDeathBlowExecutorAbility::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UDeathBlowExecutorAbility::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UDeathBlowExecutorAbility::OnMontageCanceled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UDeathBlowExecutorAbility::FindBestLocationAndRotation(AActor* Target, FVector& OutLocation, FRotator& OutRotation) const
{
	if (!IsValid(Target))
	{
		OutLocation = GetCombatCharacter()->GetActorLocation();
		OutRotation = GetCombatCharacter()->GetActorRotation();
		return;
	}

	OutLocation = Target->GetActorLocation() + (Target->GetActorForwardVector() * WarpDistance);
	OutRotation = UKismetMathLibrary::FindLookAtRotation(OutLocation, Target->GetActorLocation());
}

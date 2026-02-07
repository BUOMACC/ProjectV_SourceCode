// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_NetworkSyncPoint.h"
#include "Inventory/InventoryComponent.h"
#include "Interact/PickupActor.h"

UPickupAbility::UPickupAbility()
{
	AbilityTags.AddTag(FCombatGameplayTags::Get().Ability_Type_Action_InteractActive);
	AbilityTags.AddTag(FCombatGameplayTags::Get().ActivationType_Active);

	CancelAbilitiesWithTag.AddTag(FCombatGameplayTags::Get().Ability_Type_Action_Guard);

	ActivationRequiredTags.AddTag(FCombatGameplayTags::Get().Character_MovementMode_Walking);

	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Input_State_DisableFirstCombo);
	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Character_State_Invincible);
	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Character_State_Interact);

	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;

	InputTag = FCombatGameplayTags::Get().Input_Type_Interact;
	bAutoTriggerWhenInput = true;

	PickupMontage = nullptr;
	PlayRate = 1.f;
}

void UPickupAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!IsValid(GetSourceObject(Handle, ActorInfo)))
	{
		UE_LOG(LogTemp, Error, TEXT("USitAbility::ActivateAbility - SourceObject is nullptr."), *GetName());
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	UAbilityTask_NetworkSyncPoint* NetSyncTask = UAbilityTask_NetworkSyncPoint::WaitNetSync(this, EAbilityTaskNetSyncType::OnlyClientWait);
	NetSyncTask->OnSync.AddDynamic(this, &UPickupAbility::OnNetSync);
	NetSyncTask->ReadyForActivation();
}

bool UPickupAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UPickupAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPickupAbility::OnNetSync()
{
	const AActor* TargetActor = Cast<AActor>(GetSourceObject(CurrentSpecHandle, CurrentActorInfo));

	if (PickupMontage)
	{
		UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, PickupMontage, PlayRate, NAME_None, false, 1.0f);
		Task->ReadyForActivation();
	}

	PickupItem();

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UPickupAbility::PickupItem()
{
	if (!HasAuthority(&CurrentActivationInfo))
		return;

	UObject* SourceObject = GetSourceObject(CurrentSpecHandle, CurrentActorInfo);
	if (!IsValid(SourceObject))
	{
		return;
	}

	if (UInventoryComponent* InventoryComponent = GetAvatarActorFromActorInfo()->GetComponentByClass<UInventoryComponent>())
	{
		if (APickupActor* PickupActor = Cast<APickupActor>(SourceObject))
		{
			const TArray<FPickupInfo>& PickupInfos = PickupActor->GetPickupInfos();
			for (const FPickupInfo& PickupInfo : PickupInfos)
			{
				InventoryComponent->AddItemDefinition(PickupInfo.ItemDef, PickupInfo.Count);
			}
			PickupActor->Destroy();
		}
	}

}

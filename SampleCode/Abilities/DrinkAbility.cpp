// Fill out your copyright notice in the Description page of Project Settings.


#include "DrinkAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Inventory/UsableItemVisualizer.h"
#include "Inventory/ItemInstance.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/Fragment/ItemFragment_Useable.h"

UDrinkAbility::UDrinkAbility()
{
	AbilityTags.AddTag(FCombatGameplayTags::Get().Ability_Type_Action_Consume);
	AbilityTags.AddTag(FCombatGameplayTags::Get().ActivationType_Active);

	CancelAbilitiesWithTag.AddTag(FCombatGameplayTags::Get().Ability_Type_Action_Guard);
	CancelAbilitiesWithTag.AddTag(FCombatGameplayTags::Get().Ability_Type_Movement_Run);

	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Character_State_Consume);
	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Input_State_DisableFirstCombo);

	ActivationRequiredTags.AddTag(FCombatGameplayTags::Get().Character_MovementMode_Walking);

	bRetriggerInstancedAbility = true;

	DrinkMontage = nullptr;
	PlayRate = 1.f;
}

void UDrinkAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (DrinkMontage == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UDrinkAbility::ActivateAbility - Montage is nullptr."));
		return;
	}

	if (!IsValid(GetSourceObject(Handle, ActorInfo)))
	{
		UE_LOG(LogTemp, Error, TEXT("UDrinkAbility::ActivateAbility - Cancelled. SourceObject is nullptr."));
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	CachedItemInstance = Cast<UItemInstance>(GetSourceObject(Handle, ActorInfo));

	if (!CachedItemInstance.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("UDrinkAbility::ActivateAbility - Cancelled. SourceObject is not ItemInstance."));
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	if (CachedItemInstance->GetCount() <= 0)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, DrinkMontage, PlayRate, NAME_None, true, 1.0f);
	MontageTask->OnCompleted.AddDynamic(this, &UDrinkAbility::OnMontageCompleted);
	MontageTask->OnInterrupted.AddDynamic(this, &UDrinkAbility::OnMontageInterrupted);
	MontageTask->OnCancelled.AddDynamic(this, &UDrinkAbility::OnMontageCanceled);
	MontageTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitConsumeEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FCombatGameplayTags::Get().NotifyTiming_Consume, nullptr, true, true);
	WaitConsumeEventTask->EventReceived.AddDynamic(this, &UDrinkAbility::OnReceiveConsumeEvent);
	WaitConsumeEventTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitSpawnEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FCombatGameplayTags::Get().NotifyTiming_AfterSpawn, nullptr, true, true);
	WaitSpawnEventTask->EventReceived.AddDynamic(this, &UDrinkAbility::OnReceiveAfterSpawnEvent);
	WaitSpawnEventTask->ReadyForActivation();
}

bool UDrinkAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UDrinkAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	CachedItemInstance = nullptr;
}

void UDrinkAbility::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UDrinkAbility::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UDrinkAbility::OnMontageCanceled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UDrinkAbility::OnReceiveConsumeEvent(FGameplayEventData Payload)
{
	if (UInventoryComponent* InventoryComponent = GetCombatCharacter()->FindComponentByClass<UInventoryComponent>())
	{
		if (!CachedItemInstance.IsValid())
		{
			return;
		}

		const UItemFragment_Useable* Fragment = CachedItemInstance->FindItemFragmentByClass<UItemFragment_Useable>();
		if (Fragment == nullptr)
		{
			return;
		}

		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(Fragment->ItemEffectClass, 1.f);
		ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle);

		InventoryComponent->RemoveItemCount(CachedItemInstance.Get(), 1);
	}
}

void UDrinkAbility::OnReceiveAfterSpawnEvent(FGameplayEventData Payload)
{
	if (AUsableItemVisualizer* SpawnedActor = Cast<AUsableItemVisualizer>(Payload.OptionalObject))
	{
		if (!CachedItemInstance.IsValid())
		{
			SpawnedActor->Destroy();
			return;
		}
		SpawnedActor->SetVisual(CachedItemInstance.Get());
	}
}

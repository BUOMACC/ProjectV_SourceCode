// Fill out your copyright notice in the Description page of Project Settings.


#include "GuardHitAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UGuardHitAbility::UGuardHitAbility()
{
	AbilityTags.AddTag(FCombatGameplayTags::Get().Ability_Type_Hit_Guard);

	// 가드로 막을시 가드가 유지되어야 지속적인 막기가 가능합니다.
	CancelAbilitiesWithTag.RemoveTag(FCombatGameplayTags::Get().Ability_Type_Action_Guard);

	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Character_State_Guard);

	FAbilityTriggerData TriggerEventData;
	TriggerEventData.TriggerTag = FCombatGameplayTags::Get().HitEvent_Result_Guard;
	TriggerEventData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerEventData);

	GuardHitMontageList.Empty();
	PlayRate = 1.f;
}

void UGuardHitAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (GuardHitMontageList.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("UGuardHitAbility::ActivateAbility - Montage List Size is Empty. [%s]"), *GetName());
		return;
	}

	for (int32 Idx = 0; Idx < GuardHitMontageList.Num(); Idx++)
	{
		if (GuardHitMontageList[Idx] == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("UGuardHitAbility::ActivateAbility - [%d] Index Montage is nullptr. [%s]"), Idx, *GetName());
			return;
		}
	}

	int32 RandomIdx = FMath::RandRange(0, GuardHitMontageList.Num() - 1);
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, GuardHitMontageList[RandomIdx], PlayRate, NAME_None, true, 1.0f);
	Task->OnCompleted.AddDynamic(this, &UGuardHitAbility::OnMontageCompleted);
	Task->OnInterrupted.AddDynamic(this, &UGuardHitAbility::OnMontageInterrupted);
	Task->OnCancelled.AddDynamic(this, &UGuardHitAbility::OnMontageCanceled);
	Task->ReadyForActivation();
}

void UGuardHitAbility::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGuardHitAbility::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGuardHitAbility::OnMontageCanceled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "GuardBreakHitAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UGuardBreakHitAbility::UGuardBreakHitAbility()
{
	AbilityTags.AddTag(FCombatGameplayTags::Get().Ability_Type_Hit_GuardBreak);

	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Character_MainState_Hit);

	FAbilityTriggerData TriggerEventData;
	TriggerEventData.TriggerTag = FCombatGameplayTags::Get().HitEvent_Result_GuardBreak;
	TriggerEventData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerEventData);

	GuardBreakMontage = nullptr;
	PlayRate = 1.f;
}

void UGuardBreakHitAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (GuardBreakMontage == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UGuardBreakHitAbility::ActivateAbility - Montage is nullptr. [%s]"), *GetName());
		return;
	}

	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, GuardBreakMontage, PlayRate, NAME_None, true, 1.0f);
	Task->OnCompleted.AddDynamic(this, &UGuardBreakHitAbility::OnMontageCompleted);
	Task->OnInterrupted.AddDynamic(this, &UGuardBreakHitAbility::OnMontageInterrupted);
	Task->OnCancelled.AddDynamic(this, &UGuardBreakHitAbility::OnMontageCanceled);
	Task->ReadyForActivation();
}

void UGuardBreakHitAbility::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGuardBreakHitAbility::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGuardBreakHitAbility::OnMontageCanceled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackFailAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UAttackFailAbility::UAttackFailAbility()
{
	AbilityTags.AddTag(FCombatGameplayTags::Get().Ability_Type_Hit_AttackFailed);

	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Character_MainState_Hit);

	AttackFailMontage = nullptr;
	PlayRate = 1.f;
}

void UAttackFailAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (AttackFailMontage == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UAttackFailAbility::ActivateAbility - Montage is nullptr. [%s]"), *GetName());
		return;
	}

	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, AttackFailMontage, PlayRate, NAME_None, true, 1.0f);
	Task->OnCompleted.AddDynamic(this, &UAttackFailAbility::OnMontageCompleted);
	Task->OnInterrupted.AddDynamic(this, &UAttackFailAbility::OnMontageInterrupted);
	Task->OnCancelled.AddDynamic(this, &UAttackFailAbility::OnMontageCanceled);
	Task->ReadyForActivation();
}

void UAttackFailAbility::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UAttackFailAbility::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UAttackFailAbility::OnMontageCanceled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

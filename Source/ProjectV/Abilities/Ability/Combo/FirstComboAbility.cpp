// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstComboAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UFirstComboAbility::UFirstComboAbility()
{
	ActivationRequiredTags.AddTag(FCombatGameplayTags::Get().Character_MovementMode_Walking);

	InputTag = FCombatGameplayTags::Get().Input_Type_LightAttack;
	bAutoTriggerWhenInput = true;

	// Set Default
	FirstComboMontage = nullptr;
	PlayRate = 1.f;
}

void UFirstComboAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (FirstComboMontage == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UFirstComboAbility::ActivateAbility - Montage is nullptr. [%s]"), *GetName());
		return;
	}

	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, FirstComboMontage, PlayRate, NAME_None, true, 1.0f);
	Task->OnCompleted.AddDynamic(this, &UFirstComboAbility::OnMontageCompleted);
	Task->OnInterrupted.AddDynamic(this, &UFirstComboAbility::OnMontageInterrupted);
	Task->OnCancelled.AddDynamic(this, &UFirstComboAbility::OnMontageCanceled);
	Task->ReadyForActivation();
}

void UFirstComboAbility::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UFirstComboAbility::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UFirstComboAbility::OnMontageCanceled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

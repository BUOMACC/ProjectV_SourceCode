// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UGroundAbility::UGroundAbility()
{
	AbilityTags.AddTag(FCombatGameplayTags::Get().Ability_Type_Hit_Ground);

	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Character_MainState_Ground);

	GroundMontage = nullptr;
	PlayRate = 1.f;
}

void UGroundAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (GroundMontage == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UGroundAbility::ActivateAbility - Montage is nullptr. [%s]"), *GetName());
		return;
	}

	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, GroundMontage, PlayRate, NAME_None, true, 1.0f);
	Task->OnCompleted.AddDynamic(this, &UGroundAbility::OnMontageCompleted);
	Task->OnInterrupted.AddDynamic(this, &UGroundAbility::OnMontageInterrupted);
	Task->OnCancelled.AddDynamic(this, &UGroundAbility::OnMontageCanceled);
	Task->ReadyForActivation();
}

void UGroundAbility::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGroundAbility::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGroundAbility::OnMontageCanceled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

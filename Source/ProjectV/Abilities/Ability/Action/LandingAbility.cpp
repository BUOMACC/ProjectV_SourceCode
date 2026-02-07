// Fill out your copyright notice in the Description page of Project Settings.


#include "LandingAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

ULandingAbility::ULandingAbility()
{
	AbilityTags.AddTag(FCombatGameplayTags::Get().Ability_Type_Action_Landing);
	AbilityTags.AddTag(FCombatGameplayTags::Get().ActivationType_Active);

	ActivationRequiredTags.AddTag(FCombatGameplayTags::Get().Character_MovementMode_Walking);

	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Input_State_DisableFirstCombo);

	LandingMontage = nullptr;
	PlayRate = 1.f;
}

void ULandingAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (LandingMontage == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ULandingAbility::ActivateAbility - Montage is nullptr. [%s]"), *GetName());
		return;
	}

	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, LandingMontage, PlayRate, NAME_None, true, 1.0f);
	Task->OnCompleted.AddDynamic(this, &ULandingAbility::OnMontageCompleted);
	Task->OnInterrupted.AddDynamic(this, &ULandingAbility::OnMontageInterrupted);
	Task->OnCancelled.AddDynamic(this, &ULandingAbility::OnMontageCanceled);
	Task->ReadyForActivation();
}

bool ULandingAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void ULandingAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULandingAbility::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void ULandingAbility::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void ULandingAbility::OnMontageCanceled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

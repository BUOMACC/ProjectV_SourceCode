// Fill out your copyright notice in the Description page of Project Settings.


#include "LinkComboAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

ULinkComboAbility::ULinkComboAbility()
{
	ActivationRequiredTags.AddTag(FCombatGameplayTags::Get().Character_MovementMode_Walking);

	// Set Default
	LinkComboMontage = nullptr;
	PlayRate = 1.f;
}

void ULinkComboAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (LinkComboMontage == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ULinkComboAbility::ActivateAbility - Montage is nullptr. [%s]"), *GetName());
		return;
	}

	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, LinkComboMontage, PlayRate, NAME_None, true, 1.0f);
	Task->OnCompleted.AddDynamic(this, &ULinkComboAbility::OnMontageCompleted);
	Task->OnInterrupted.AddDynamic(this, &ULinkComboAbility::OnMontageInterrupted);
	Task->OnCancelled.AddDynamic(this, &ULinkComboAbility::OnMontageCanceled);
	Task->ReadyForActivation();
}

void ULinkComboAbility::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void ULinkComboAbility::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void ULinkComboAbility::OnMontageCanceled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

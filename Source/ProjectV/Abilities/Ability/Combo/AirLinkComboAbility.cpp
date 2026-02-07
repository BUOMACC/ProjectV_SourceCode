// Fill out your copyright notice in the Description page of Project Settings.


#include "AirLinkComboAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UAirLinkComboAbility::UAirLinkComboAbility()
{
	// 점프중에 공중 공격을 허용해야 하기 때문에, 점프 태그를 제외합니다.
	ActivationBlockedTags.RemoveTag(FCombatGameplayTags::Get().Character_State_Jump);

	ActivationRequiredTags.AddTag(FCombatGameplayTags::Get().Character_MovementMode_Falling);

	// Set Default
	AirLinkComboMontage = nullptr;
	PlayRate = 1.f;
}

void UAirLinkComboAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (AirLinkComboMontage == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UAirLinkComboAbility::ActivateAbility - Montage is nullptr. [%s]"), *GetName());
		return;
	}

	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, AirLinkComboMontage, PlayRate, NAME_None, true, 1.0f);
	Task->OnCompleted.AddDynamic(this, &UAirLinkComboAbility::OnMontageCompleted);
	Task->OnInterrupted.AddDynamic(this, &UAirLinkComboAbility::OnMontageInterrupted);
	Task->OnCancelled.AddDynamic(this, &UAirLinkComboAbility::OnMontageCanceled);
	Task->ReadyForActivation();
}

void UAirLinkComboAbility::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UAirLinkComboAbility::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UAirLinkComboAbility::OnMontageCanceled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

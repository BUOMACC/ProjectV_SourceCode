// Fill out your copyright notice in the Description page of Project Settings.


#include "AirFirstComboAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UAirFirstComboAbility::UAirFirstComboAbility()
{
	// 점프중에 공중 공격을 허용해야 하기 때문에, 점프 태그를 제외합니다.
	ActivationBlockedTags.RemoveTag(FCombatGameplayTags::Get().Character_State_Jump);

	ActivationRequiredTags.AddTag(FCombatGameplayTags::Get().Character_MovementMode_Falling);

	InputTag = FCombatGameplayTags::Get().Input_Type_LightAttack;
	bAutoTriggerWhenInput = true;

	// Set Default
	AirFirstComboMontage = nullptr;
	PlayRate = 1.f;
}

void UAirFirstComboAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (AirFirstComboMontage == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UAirFirstComboAbility::ActivateAbility - Montage is nullptr. [%s]"), *GetName());
		return;
	}

	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, AirFirstComboMontage, PlayRate, NAME_None, true, 1.0f);
	Task->OnCompleted.AddDynamic(this, &UAirFirstComboAbility::OnMontageCompleted);
	Task->OnInterrupted.AddDynamic(this, &UAirFirstComboAbility::OnMontageInterrupted);
	Task->OnCancelled.AddDynamic(this, &UAirFirstComboAbility::OnMontageCanceled);
	Task->ReadyForActivation();
}

void UAirFirstComboAbility::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UAirFirstComboAbility::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UAirFirstComboAbility::OnMontageCanceled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

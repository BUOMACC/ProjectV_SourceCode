// Fill out your copyright notice in the Description page of Project Settings.


#include "KnockdownAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UKnockdownAbility::UKnockdownAbility()
{
	AbilityTags.AddTag(FCombatGameplayTags::Get().Ability_Type_Hit_Knockdown);

	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Character_MainState_Ground);

	FAbilityTriggerData TriggerEventData;
	TriggerEventData.TriggerTag = FCombatGameplayTags::Get().HitEvent_Result_Knockdown;
	TriggerEventData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerEventData);

	KnockdownMontage = nullptr;
	PlayRate = 1.f;
}

void UKnockdownAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (KnockdownMontage == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UKnockdownAbility::ActivateAbility - Montage is nullptr. [%s]"), *GetName());
		return;
	}

	/*
	 *	누워있는 상태가 유지되지 않으면, 핑이 높은 멀티플레이 환경에서 바로 일어나는 부자연스러운 동작이 보여질 수 있습니다.
	 *	비정상적으로 몽타주가 취소되거나, 다른 몽타주에 의해 전환시 어빌리티를 종료합니다.
	 */
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, KnockdownMontage, PlayRate, NAME_None, true, 1.0f);
	Task->OnInterrupted.AddDynamic(this, &UKnockdownAbility::OnMontageInterrupted);
	Task->OnCancelled.AddDynamic(this, &UKnockdownAbility::OnMontageCanceled);
	Task->ReadyForActivation();
}

void UKnockdownAbility::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UKnockdownAbility::OnMontageCanceled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

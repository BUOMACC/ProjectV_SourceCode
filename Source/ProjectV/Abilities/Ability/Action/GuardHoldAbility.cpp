// Fill out your copyright notice in the Description page of Project Settings.


#include "GuardHoldAbility.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UGuardHoldAbility::UGuardHoldAbility()
{
	AbilityTags.AddTag(FCombatGameplayTags::Get().ActivationType_Active);
	AbilityTags.AddTag(FCombatGameplayTags::Get().Ability_Type_Action_Guard);

	CancelAbilitiesWithTag.AddTag(FCombatGameplayTags::Get().Ability_Type_Movement_Run);

	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Character_State_Guard);
	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Input_State_DisableFirstCombo);

	bRetriggerInstancedAbility = true;

	GuardStopMontage = nullptr;
	PlayRate = 1.f;
}

void UGuardHoldAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 1) 가드입력 해제 대기 태스크 추가
	UAbilityTask_WaitInputRelease* Task = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	Task->OnRelease.AddDynamic(this, &UGuardHoldAbility::OnInputRelease);
	Task->ReadyForActivation();
}

bool UGuardHoldAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGuardHoldAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGuardHoldAbility::OnInputRelease(float TimeHeld)
{
	// 가드를 내리는 모션이 없으면, 그냥 어빌리티를 끝내서 Idle 애니메이션이랑 블렌딩되게 둡니다.
	if (GuardStopMontage == nullptr)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	// Play Guard End Animation
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, GuardStopMontage, PlayRate, NAME_None, false, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &UGuardHoldAbility::OnGuardStopMontageBlendingOut);
	Task->OnInterrupted.AddDynamic(this, &UGuardHoldAbility::OnGuardStopMontageInterrupted);
	Task->OnCancelled.AddDynamic(this, &UGuardHoldAbility::OnGuardStopMontageCacncelled);
	Task->ReadyForActivation();
}

void UGuardHoldAbility::OnGuardStopMontageBlendingOut()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGuardHoldAbility::OnGuardStopMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGuardHoldAbility::OnGuardStopMontageCacncelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGuardHoldAbility::ForceRelease()
{
	OnInputRelease(0.f);
}
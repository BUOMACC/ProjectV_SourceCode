// Fill out your copyright notice in the Description page of Project Settings.


#include "GuardStartAbility.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/CombatCharacter.h"
#include "CombatMovementComponent.h"

UGuardStartAbility::UGuardStartAbility()
{
	AbilityTags.AddTag(FCombatGameplayTags::Get().ActivationType_Active);
	AbilityTags.AddTag(FCombatGameplayTags::Get().Ability_Type_Action_Guard);

	CancelAbilitiesWithTag.AddTag(FCombatGameplayTags::Get().Ability_Type_Movement_Run);
	CancelAbilitiesWithTag.AddTag(FCombatGameplayTags::Get().Ability_Type_Movement_Crouch);

	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Character_State_Guard);
	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Input_State_DisableFirstCombo);

	ActivationBlockedTags.AddTag(FCombatGameplayTags::Get().Character_MainState_Groggy);
	ActivationBlockedTags.AddTag(FCombatGameplayTags::Get().Character_MainState_Dead);

	InputTag = FCombatGameplayTags::Get().Input_Type_Guard;
	bAutoTriggerWhenInput = true;

	GuardStartMontage = nullptr;
	GuardStopMontage = nullptr;
	PlayRate = 1.f;
}

void UGuardStartAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (GuardStartMontage == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UGuardStartAbility::ActivateAbility - Montage is nullptr. [%s]"), *GetName());
		return;
	}

	// * Play Guard Start Animation
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, GuardStartMontage, PlayRate, NAME_None, true, 1.0f);
	PlayMontageTask->OnCompleted.AddDynamic(this, &UGuardStartAbility::OnGuardStartMontageCompleted);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &UGuardStartAbility::OnGuardStartMontageInterrupted);
	PlayMontageTask->OnCancelled.AddDynamic(this, &UGuardStartAbility::OnGuardStartMontageCanceled);
	PlayMontageTask->ReadyForActivation();

	// * Wait Guard Input Release
	UAbilityTask_WaitInputRelease* WaitInputTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, false);
	WaitInputTask->OnRelease.AddDynamic(this, &UGuardStartAbility::OnInputRelease);
	WaitInputTask->ReadyForActivation();

	// * Apply Parry Effect
	UGameplayEffect* GameplayEffectCDO = GameplayEffectToApply.GetDefaultObject();
	ApplyGameplayEffectToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, GameplayEffectCDO, 1.f);

	GetCombatCharacter()->UnCrouch();
}

bool UGuardStartAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	return bResult && GetCombatCharacter()->GetCombatMovementComponent()->CanUnCrouch();
}

void UGuardStartAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGuardStartAbility::OnInputRelease(float TimeHeld)
{
	// 가드를 내리는 모션이 없으면, 그냥 어빌리티를 끝내서 Idle 애니메이션이랑 블렌딩되게 둡니다.
	if (GuardStopMontage == nullptr)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	// Play Guard End Animation
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, GuardStopMontage, PlayRate, NAME_None, false, 1.0f);
	Task->OnCompleted.AddDynamic(this, &UGuardStartAbility::OnGuardStopMontageCompleted);
	Task->OnInterrupted.AddDynamic(this, &UGuardStartAbility::OnGuardStopMontageInterrupted);
	Task->OnCancelled.AddDynamic(this, &UGuardStartAbility::OnGuardStopMontageCacncelled);
	Task->ReadyForActivation();
}

void UGuardStartAbility::OnGuardStartMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGuardStartAbility::OnGuardStartMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGuardStartAbility::OnGuardStartMontageCanceled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGuardStartAbility::OnGuardStopMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGuardStartAbility::OnGuardStopMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGuardStartAbility::OnGuardStopMontageCacncelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
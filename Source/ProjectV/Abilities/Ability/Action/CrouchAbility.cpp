// Fill out your copyright notice in the Description page of Project Settings.


#include "CrouchAbility.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"

UCrouchAbility::UCrouchAbility()
{
	// Tags
	AbilityTags.AddTag(FCombatGameplayTags::Get().Ability_Type_Movement_Crouch);

	CancelAbilitiesWithTag.AddTag(FCombatGameplayTags::Get().Ability_Type_Movement_Run);

	BlockAbilitiesWithTag.AddTag(FCombatGameplayTags::Get().Ability_Type_Movement_Run);

	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Character_State_Crouch);

	ActivationBlockedTags.AddTag(FCombatGameplayTags::Get().Character_MainState_Airborne);
	ActivationBlockedTags.AddTag(FCombatGameplayTags::Get().Character_MainState_Ground);
	ActivationBlockedTags.AddTag(FCombatGameplayTags::Get().Character_MainState_Dead);
	ActivationBlockedTags.AddTag(FCombatGameplayTags::Get().Character_MainState_Hit);
	ActivationBlockedTags.AddTag(FCombatGameplayTags::Get().Character_MovementMode_Falling);
	ActivationBlockedTags.AddTag(FCombatGameplayTags::Get().Character_State_Attack);
	ActivationBlockedTags.AddTag(FCombatGameplayTags::Get().Character_State_Guard);
	ActivationBlockedTags.AddTag(FCombatGameplayTags::Get().Character_State_Dodge);
	ActivationBlockedTags.AddTag(FCombatGameplayTags::Get().Character_State_Skill);

	// Inputs
	InputTag = FCombatGameplayTags::Get().Input_Type_Crouch;
	bAutoTriggerWhenInput = true;
}

void UCrouchAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	GetCombatCharacter()->Crouch();
	GetCombatCharacter()->OnCharacterMovementUpdated.AddDynamic(this, &UCrouchAbility::OnMovementUpdated);

	UAbilityTask_WaitInputPress* Task = UAbilityTask_WaitInputPress::WaitInputPress(this, false);
	Task->OnPress.AddDynamic(this, &UCrouchAbility::OnInputPress);
	Task->ReadyForActivation();

	ApplyCooldown(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
}

bool UCrouchAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	return bResult && GetCombatCharacter()->CanCrouch();
}

void UCrouchAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	GetCombatCharacter()->OnCharacterMovementUpdated.RemoveDynamic(this, &UCrouchAbility::OnMovementUpdated);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UCrouchAbility::OnInputPress(float TimeWaited)
{
	GetCombatCharacter()->UnCrouch();
}

void UCrouchAbility::OnMovementUpdated(float DeltaSeconds, FVector OldLocation, FVector OldVelocity)
{
	// 천장이 너무 낮는 등의 이유로 일어서기에 실패할수 있으므로 OnMovementUpdated에서 체크하여 어빌리티를 종료해야 합니다.
	if (GetCombatCharacter()->bIsCrouched == false)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

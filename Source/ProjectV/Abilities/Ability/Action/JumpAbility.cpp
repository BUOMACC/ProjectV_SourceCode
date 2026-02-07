// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpAbility.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Abilities/Tasks/AbilityTask_WaitMovementModeChange.h"

UJumpAbility::UJumpAbility()
{
	AbilityTags.AddTag(FCombatGameplayTags::Get().Ability_Type_Movement_Jump);

	BlockAbilitiesWithTag.AddTag(FCombatGameplayTags::Get().Ability_Type_Action_Guard);
	BlockAbilitiesWithTag.AddTag(FCombatGameplayTags::Get().Ability_Type_Movement);

	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Character_State_Jump);

	ActivationBlockedTags.AddTag(FCombatGameplayTags::Get().Character_MainState_Airborne);
	ActivationBlockedTags.AddTag(FCombatGameplayTags::Get().Character_MainState_Dead);
	ActivationBlockedTags.AddTag(FCombatGameplayTags::Get().Character_MainState_Ground);
	ActivationBlockedTags.AddTag(FCombatGameplayTags::Get().Character_MainState_Hit);
	ActivationBlockedTags.AddTag(FCombatGameplayTags::Get().Character_State_Attack);
	ActivationBlockedTags.AddTag(FCombatGameplayTags::Get().Character_State_Guard);
	ActivationBlockedTags.AddTag(FCombatGameplayTags::Get().Character_State_Dodge);
	ActivationBlockedTags.AddTag(FCombatGameplayTags::Get().Character_State_Skill);

	InputTag = FCombatGameplayTags::Get().Input_Type_Jump;
	bAutoTriggerWhenInput = true;
}

void UJumpAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
	{
		Character->UnCrouch();
		Character->Jump();

		UAbilityTask_WaitInputRelease* Task_InputRelease = UAbilityTask_WaitInputRelease::WaitInputRelease(this, false);
		Task_InputRelease->OnRelease.AddDynamic(this, &UJumpAbility::OnInputRelease);
		Task_InputRelease->ReadyForActivation();

		UAbilityTask_WaitMovementModeChange* Task_MovementModeChange = UAbilityTask_WaitMovementModeChange::CreateWaitMovementModeChange(this, EMovementMode::MOVE_Walking);
		Task_MovementModeChange->OnChange.AddDynamic(this, &UJumpAbility::OnMovementModeChanged);
		Task_MovementModeChange->ReadyForActivation();

		ApplyCooldown(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
	}
}

bool UJumpAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	return bResult;
}

void UJumpAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UJumpAbility::OnInputRelease(float TimeHeld)
{
	if (ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
	{
		Character->StopJumping();
	}
}

void UJumpAbility::OnMovementModeChanged(EMovementMode NewMovementMode)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}


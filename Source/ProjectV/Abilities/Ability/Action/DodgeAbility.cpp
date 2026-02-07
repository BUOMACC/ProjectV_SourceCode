// Fill out your copyright notice in the Description page of Project Settings.


#include "DodgeAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/CombatCharacter.h"
#include "CombatMovementComponent.h"

UDodgeAbility::UDodgeAbility()
{
	AbilityTags.AddTag(FCombatGameplayTags::Get().Ability_Type_Movement_Dodge);
	AbilityTags.AddTag(FCombatGameplayTags::Get().ActivationType_Active);

	CancelAbilitiesWithTag.AddTag(FCombatGameplayTags::Get().Ability_Type_Action_Guard);
	CancelAbilitiesWithTag.AddTag(FCombatGameplayTags::Get().Ability_Type_Movement_Run);
	CancelAbilitiesWithTag.AddTag(FCombatGameplayTags::Get().Ability_Type_Movement_Crouch);

	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Character_State_IgnoreLockon);
	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Character_State_Dodge);
	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Input_State_DisableFirstCombo);

	ActivationRequiredTags.AddTag(FCombatGameplayTags::Get().Character_MovementMode_Walking);

	bRetriggerInstancedAbility = true;

	DodgeMontage = nullptr;
	PlayRate = 1.f;
}

void UDodgeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (DodgeMontage == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UDodgeAbility::ActivateAbility - Montage is nullptr. [%s]"), *GetName());
		return;
	}

	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, DodgeMontage, PlayRate, NAME_None, true, 1.0f);
	Task->OnCompleted.AddDynamic(this, &UDodgeAbility::OnMontageCompleted);
	Task->OnInterrupted.AddDynamic(this, &UDodgeAbility::OnMontageInterrupted);
	Task->OnCancelled.AddDynamic(this, &UDodgeAbility::OnMontageCanceled);
	Task->ReadyForActivation();

	GetCombatCharacter()->UnCrouch();

	if (DodgeEffect)
	{
		UGameplayEffect* GameplayEffectCDO = DodgeEffect.GetDefaultObject();
		ApplyGameplayEffectToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, GameplayEffectCDO, 1.f, 1);
	}
}

bool UDodgeAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	return bResult && GetCombatCharacter()->GetCombatMovementComponent()->CanUnCrouch();
}

void UDodgeAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDodgeAbility::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UDodgeAbility::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UDodgeAbility::OnMontageCanceled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
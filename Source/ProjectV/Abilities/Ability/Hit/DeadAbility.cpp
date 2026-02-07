// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UDeadAbility::UDeadAbility()
{
	AbilityTags.AddTag(FCombatGameplayTags::Get().Ability_Type_Dead);

	CancelAbilitiesWithTag.RemoveTag(FCombatGameplayTags::Get().Ability_Type_Action_Guard);
	CancelAbilitiesWithTag.AddTag(FCombatGameplayTags::Get().Ability_Type_Lockon);

	BlockAbilitiesWithTag.AddTag(FCombatGameplayTags::Get().Ability_Type_Attack);
	BlockAbilitiesWithTag.AddTag(FCombatGameplayTags::Get().Ability_Type_Skill);
	BlockAbilitiesWithTag.AddTag(FCombatGameplayTags::Get().Ability_Type_Lockon);
	BlockAbilitiesWithTag.AddTag(FCombatGameplayTags::Get().Ability_Type_Action);
	BlockAbilitiesWithTag.AddTag(FCombatGameplayTags::Get().Ability_Type_Hit);

	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Character_MainState_Dead);
	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Character_State_IgnoreLockon);

	FAbilityTriggerData TriggerEventData;
	TriggerEventData.TriggerTag = FCombatGameplayTags::Get().Event_Dead;
	TriggerEventData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerEventData);

	DeadMontageList.Empty();
	PlayRate = 1.f;
}

void UDeadAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// * Check Validation
	if (DeadMontageList.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("UDeadAbility::ActivateAbility - Montage is nullptr. [%s]"), *GetName());
		return;
	}

	for (int32 Idx = 0; Idx < DeadMontageList.Num(); Idx++)
	{
		const UAnimMontage* DeadMontage = DeadMontageList[Idx];
		if (DeadMontage == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("UDeadAbility::ActivateAbility - [%d] Index Montage is nullptr. [%s]"), Idx, *GetName());
			return;
		}
	}

	ACombatCharacter* CombatCharacter = GetCombatCharacter();
	if (IsValid(CombatCharacter) == false)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}

	// * Play Montage
	int32 RandomIdx = FMath::RandRange(0, DeadMontageList.Num() - 1);
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, DeadMontageList[RandomIdx], PlayRate, NAME_None, true, 1.0f);
	Task->ReadyForActivation();

	// * Call Dead Event
	CombatCharacter->Dead();
}
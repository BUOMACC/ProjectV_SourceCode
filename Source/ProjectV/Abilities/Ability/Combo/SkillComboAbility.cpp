// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Ability/Combo/SkillComboAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Equipment/EquipmentComponent.h"
#include "Equipment/EquipmentInstance.h"

USkillComboAbility::USkillComboAbility()
{
	AbilityTags.AddTag(FCombatGameplayTags::Get().Ability_Type_Skill);

	ActivationRequiredTags.AddTag(FCombatGameplayTags::Get().Character_MovementMode_Walking);

	InputTag = FCombatGameplayTags::Get().Input_Type_Skill;
	bAutoTriggerWhenInput = true;

	FAbilityTriggerData TriggerEventData;
	TriggerEventData.TriggerTag = FCombatGameplayTags::Get().Ability_Type_Skill;
	TriggerEventData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerEventData);

	// Set Default
	SkillMontage = nullptr;
	PlayRate = 1.f;
}

void USkillComboAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (SkillMontage == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("USkillComboAbility::ActivateAbility - Montage is nullptr. [%s]"), *GetName());
		return;
	}

	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, SkillMontage, PlayRate, NAME_None, true, 1.0f);
	Task->OnCompleted.AddDynamic(this, &USkillComboAbility::OnMontageCompleted);
	Task->OnInterrupted.AddDynamic(this, &USkillComboAbility::OnMontageInterrupted);
	Task->OnCancelled.AddDynamic(this, &USkillComboAbility::OnMontageCanceled);
	Task->ReadyForActivation();
}

bool USkillComboAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	bool bHasEquipment = false;
	if (ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor))
	{
		if (UEquipmentComponent* EquipmentComponent = Character->FindComponentByClass<UEquipmentComponent>())
		{
			const TArray<UEquipmentInstance*> Equipments = EquipmentComponent->GetEquipmentInstancesOfType(UEquipmentInstance::StaticClass());
			bHasEquipment = (Equipments.Num() > 0);
		}
	}

	return bResult && bHasEquipment;
}

void USkillComboAbility::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USkillComboAbility::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USkillComboAbility::OnMontageCanceled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}


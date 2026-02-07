// Fill out your copyright notice in the Description page of Project Settings.


#include "GroggyAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UGroggyAbility::UGroggyAbility()
{
	AbilityTags.AddTag(FCombatGameplayTags::Get().Ability_Type_Hit_Groggy);

	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Character_MainState_Groggy);
	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Character_State_SuperArmor);

	BlockAbilitiesWithTag.AddTag(FCombatGameplayTags::Get().Ability_Type_Action_Guard);

	FAbilityTriggerData TriggerEventData;
	TriggerEventData.TriggerTag = FCombatGameplayTags::Get().HitEvent_Result_Groggy;
	TriggerEventData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerEventData);

	GroggyMontage = nullptr;
	PlayRate = 1.f;
	GroggyEffect = nullptr;
}

void UGroggyAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (GroggyMontage == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UGroggyAbility::ActivateAbility - Montage is nullptr. [%s]"), *GetName());
		return;
	}

	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, GroggyMontage, PlayRate, NAME_None, true, 1.0f);
	Task->OnCompleted.AddDynamic(this, &UGroggyAbility::OnMontageCompleted);
	Task->OnInterrupted.AddDynamic(this, &UGroggyAbility::OnMontageInterrupted);
	Task->OnCancelled.AddDynamic(this, &UGroggyAbility::OnMontageCanceled);
	Task->ReadyForActivation();

	if (GroggyEffect)
	{
		UGameplayEffect* GameplayEffectCDO = GroggyEffect.GetDefaultObject();
		ApplyGameplayEffectToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, GameplayEffectCDO, 1.f, 1);
	}
}

void UGroggyAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (!bWasCancelled)
	{
		if (GroggyEndEffect)
		{
			UGameplayEffect* GameplayEffectCDO = GroggyEndEffect.GetDefaultObject();
			ApplyGameplayEffectToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, GameplayEffectCDO, 1.f, 1);
		}
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGroggyAbility::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGroggyAbility::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGroggyAbility::OnMontageCanceled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
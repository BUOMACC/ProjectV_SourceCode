// Fill out your copyright notice in the Description page of Project Settings.


#include "ParrySuccessAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/AttributeSet/CombatAttributeSet.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/GameState.h"

UParrySuccessAbility::UParrySuccessAbility()
{
	// 패링시 가드가 유지되어야 지속적인 막기가 가능합니다.
	CancelAbilitiesWithTag.RemoveTag(FCombatGameplayTags::Get().Ability_Type_Action_Guard);

	AbilityTags.AddTag(FCombatGameplayTags::Get().Ability_Type_Hit_Parry);

	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Character_State_Parry);

	FAbilityTriggerData TriggerEventData;
	TriggerEventData.TriggerTag = FCombatGameplayTags::Get().HitEvent_Result_Parry;
	TriggerEventData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerEventData);

	ParrySuccessMontages.Empty();
	PlayRate = 1.f;
}

void UParrySuccessAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// * Check Validation
	if (ParrySuccessMontages.Montages.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("UAttackFailAbility::ActivateAbility - Montage is nullptr. [%s]"), *GetName());
		return;
	}


	for (int32 Idx = 0; Idx < ParrySuccessMontages.Montages.Num() ; Idx++)
	{
		const UAnimMontage* ParrySuccessMontage = ParrySuccessMontages.Montages[Idx];
		if (ParrySuccessMontage == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("UParrySuccessAbility::ActivateAbility - [%d] Index Montage is nullptr. [%s]"), Idx, *GetName());
			return;
		}
	}

	// * Play Montage
	UAnimMontage* ParryMontage = ParrySuccessMontages.GetRandomMontage(GetWorld()->GetGameState()->GetServerWorldTimeSeconds());
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, ParryMontage, PlayRate, NAME_None, true, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &UParrySuccessAbility::OnMontageBlendOut);
	Task->OnInterrupted.AddDynamic(this, &UParrySuccessAbility::OnMontageInterrupted);
	Task->OnCancelled.AddDynamic(this, &UParrySuccessAbility::OnMontageCanceled);
	Task->ReadyForActivation();

	// * Try Activate React Ability to Attacker & Apply ParryEffect (GE)
	const FGameplayEffectContextHandle& ContextHandle = CurrentEventData.ContextHandle;

	UAbilitySystemComponent* AttackerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAttacker());
	check(AttackerASC);

	// 1) 패링당한 대상에게 공격실패 반응 어빌리티 실행
	TSubclassOf<UGameplayAbility> ParryAbility = GetHitData()->ParryAbility;
	if (ParryAbility)
	{
		AttackerASC->TryActivateAbilityByClass(ParryAbility);
	}

	// 2) 패링당한 대상의 그로기 수치 증가
	if (DummyEffect)
	{
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(DummyEffect, 1.f);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, FCombatGameplayTags::Get().SetByCaller_GroggyGauge, GetHitData()->GroggyGauge * 0.5f);

		ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(GetAttacker()));
	}
}

void UParrySuccessAbility::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UParrySuccessAbility::OnMontageBlendOut()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UParrySuccessAbility::OnMontageCanceled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

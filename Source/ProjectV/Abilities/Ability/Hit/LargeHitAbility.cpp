// Fill out your copyright notice in the Description page of Project Settings.


#include "LargeHitAbility.h"
#include "FunctionLibrary/CommonFunctionLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

ULargeHitAbility::ULargeHitAbility()
{
	AbilityTags.AddTag(FCombatGameplayTags::Get().Ability_Type_Hit_Large);

	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Character_MainState_Hit);

	FAbilityTriggerData TriggerEventData;
	TriggerEventData.TriggerTag = FCombatGameplayTags::Get().HitEvent_Result_Large;
	TriggerEventData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerEventData);

	PlayRate = 1.f;
}

void ULargeHitAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// * Check Validation
	for (int32 Idx = 0; Idx < (int32)EHitDirectionType::End; Idx++)
	{
		const UAnimMontage* LargeHitMontage = LargeHitMontageList[Idx];
		if (LargeHitMontage == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("ULargeHitAbility::ActivateAbility - [%s] Index Montage is nullptr. [%s]"), *UCommonFunctionLibrary::GetEnumNameString<EHitDirectionType>(Idx), *GetName());
			return;
		}
	}

	// * Play Montage
	UAnimMontage* BestLargeHitMontage = LargeHitMontageList[(int32)GetHitDirectionType()];
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, BestLargeHitMontage, PlayRate, NAME_None, true, 1.0f);
	Task->OnCompleted.AddDynamic(this, &ULargeHitAbility::OnMontageCompleted);
	Task->OnInterrupted.AddDynamic(this, &ULargeHitAbility::OnMontageInterrupted);
	Task->OnCancelled.AddDynamic(this, &ULargeHitAbility::OnMontageCanceled);
	Task->ReadyForActivation();
}

void ULargeHitAbility::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void ULargeHitAbility::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void ULargeHitAbility::OnMontageCanceled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

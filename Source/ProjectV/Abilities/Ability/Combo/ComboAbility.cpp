// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "CombatMovementComponent.h"
#include "Interface/DamageInterface.h"

UComboAbility::UComboAbility()
{
	AbilityTags.AddTag(FCombatGameplayTags::Get().Ability_Type_Attack);
	AbilityTags.AddTag(FCombatGameplayTags::Get().ActivationType_Active);

	CancelAbilitiesWithTag.AddTag(FCombatGameplayTags::Get().Ability_Type_Action_Guard);
	CancelAbilitiesWithTag.AddTag(FCombatGameplayTags::Get().Ability_Type_Movement_Run);

	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Character_State_Attack);
	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Character_State_BlockMovement);
	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Input_State_DisableFirstCombo);

	ActivationBlockedTags.AddTag(FCombatGameplayTags::Get().Character_MainState_Dead);
	ActivationBlockedTags.AddTag(FCombatGameplayTags::Get().Character_MainState_Hit);
	ActivationBlockedTags.AddTag(FCombatGameplayTags::Get().Character_MainState_Airborne);
	ActivationBlockedTags.AddTag(FCombatGameplayTags::Get().Character_MainState_Ground);
	ActivationBlockedTags.AddTag(FCombatGameplayTags::Get().Character_MainState_Groggy);
	ActivationBlockedTags.AddTag(FCombatGameplayTags::Get().Character_State_Jump);

	ActivationBlockedTags.AddTag(FCombatGameplayTags::Get().Character_State_Executable);
	ActivationBlockedTags.AddTag(FCombatGameplayTags::Get().Character_State_Execution);

	bRetriggerInstancedAbility = true;
}

void UComboAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Reset HitMap
	if (HasAuthority(&CurrentActivationInfo))
	{
		ResetHitMap();
	}

	GetCombatCharacter()->UnCrouch();
}

bool UComboAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	return bResult && GetCombatCharacter()->GetCombatMovementComponent()->CanUnCrouch();
}

void UComboAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UComboAbility::OnEventReceived(FGameplayTag MatchingTag, const FGameplayEventData* Payload)
{
	Super::OnEventReceived(MatchingTag, Payload);

	FGameplayTag EventTag = FGameplayTag::EmptyTag;
	FGameplayAbilityTargetDataHandle TargetData;
	if (Payload)
	{
		/*
		*	Extract EventTag(각 상황에 따라 받는 태그)
		*	- Cancel				: Input.{ }
		*	- SuccessOverlapHitBox	: X
		*/
		EventTag = Payload->EventTag;
		TargetData = Payload->TargetData;
	}

	if (MatchingTag.MatchesTagExact(FCombatGameplayTags::Get().Event_Combat_SuccessOverlapHitBox))
	{
		TObjectPtr<UHitData> HitData = nullptr;
		if (Payload->OptionalObject)
		{
			HitData = Cast<UHitData>(Payload->OptionalObject);
		}
		OnReceiveEvent_SuccessOverlapHitBox(TargetData, HitData);
	}
}

void UComboAbility::OnReceiveEvent_SuccessOverlapHitBox(FGameplayAbilityTargetDataHandle TargetData, UHitData* HitData)
{
	if (!CheckAlreadyHit(HitData, TargetData))
	{
		AddHitMap(HitData, TargetData);

		int32 FirstTargetIndex = 0;
		FHitResult FirstHitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetData, FirstTargetIndex);
		IDamageInterface::Execute_Damage(FirstHitResult.GetActor(), GetAbilitySystemComponentFromActorInfo(), TargetData, HitData);
	}

	K2_OnReceiveEvent_SuccessOverlapHitBox(TargetData, HitData);
}

void UComboAbility::ResetHitMap()
{
	HitMap.Empty();
}

bool UComboAbility::CheckAlreadyHit(const UHitData* HitData, const FGameplayAbilityTargetDataHandle& TargetData)
{
	if (HitData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UComboAbility::CheckAlreadyHit - HitData is nullptr. [%s]"), *GetName());
		return false;
	}

	if (FActorArray* HitActorArrayPtr = HitMap.Find(HitData))
	{
		int32 FirstTargetIndex = 0;
		FHitResult FirstHitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetData, FirstTargetIndex);
		if (AActor* TargetActor = FirstHitResult.GetActor())
		{
			// if found, return true (already hit actor)
			return HitActorArrayPtr->Actors.Contains(TargetActor);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UComboAbility::CheckAlreadyHit - Target Actor not found. [%s]"), *GetName());
			return false;
		}
	}

	return false;
}

bool UComboAbility::AddHitMap(const UHitData* HitData, const FGameplayAbilityTargetDataHandle& TargetData)
{
	bool bSuccessToAdd = false;

	int32 FirstTargetIndex = 0;
	FHitResult FirstHitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetData, FirstTargetIndex);
	if (AActor* TargetActor = FirstHitResult.GetActor())
	{
		FActorArray& HitActorArrayPtr = HitMap.FindOrAdd(const_cast<UHitData*>(HitData));
		HitActorArrayPtr.Actors.AddUnique(TargetActor);
		bSuccessToAdd = true;
	}

	return bSuccessToAdd;
}
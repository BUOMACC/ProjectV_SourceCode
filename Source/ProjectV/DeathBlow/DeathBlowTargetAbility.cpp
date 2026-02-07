// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathBlowTargetAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/CapsuleComponent.h"

UDeathBlowTargetAbility::UDeathBlowTargetAbility()
{
	AbilityTags.AddTag(FCombatGameplayTags::Get().Ability_Type_Hit_DeathBlow_Default);
	AbilityTags.AddTag(FCombatGameplayTags::Get().ActivationType_Active);

	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Input_State_DisableFirstCombo);
	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Character_State_Invincible);
	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Character_State_DeathBlow);

	ActivationRequiredTags.AddTag(FCombatGameplayTags::Get().Character_MovementMode_Walking);

	// Default Values
	FAbilityTriggerData TriggerEventData;
	TriggerEventData.TriggerTag = FCombatGameplayTags::Get().Ability_Type_Hit_DeathBlow_Default;
	TriggerEventData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerEventData);

	bRetriggerInstancedAbility = true;

	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
}

void UDeathBlowTargetAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (DeathBlowTargetMontage == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UDeathBlowTargetAbility::ActivateAbility - Montage is nullptr. [%s]"), *GetName());
		return;
	}

	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, DeathBlowTargetMontage, 1.f, NAME_None, true, 1.0f);
	MontageTask->OnCompleted.AddDynamic(this, &UDeathBlowTargetAbility::OnMontageCompleted);
	MontageTask->OnInterrupted.AddDynamic(this, &UDeathBlowTargetAbility::OnMontageInterrupted);
	MontageTask->OnCancelled.AddDynamic(this, &UDeathBlowTargetAbility::OnMontageCanceled);
	MontageTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitDeathBlowEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FCombatGameplayTags::Get().NotifyTiming_DeathBlow, nullptr, true, true);
	WaitDeathBlowEventTask->EventReceived.AddDynamic(this, &UDeathBlowTargetAbility::OnReceiveDeathBlowEvent);
	WaitDeathBlowEventTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitDeathBlowRestoreEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FCombatGameplayTags::Get().NotifyTiming_DeathBlowRestore, nullptr, true, true);
	WaitDeathBlowRestoreEventTask->EventReceived.AddDynamic(this, &UDeathBlowTargetAbility::OnReceiveDeathBlowRestoreEvent);
	WaitDeathBlowRestoreEventTask->ReadyForActivation();

	//GetCombatCharacter()->GetCapsuleComponent()->SetCollisionProfileName(TEXT("IgnoreCharacter"));
}

bool UDeathBlowTargetAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UDeathBlowTargetAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	//GetCombatCharacter()->GetCapsuleComponent()->SetCollisionProfileName(TEXT("Character"));

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDeathBlowTargetAbility::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UDeathBlowTargetAbility::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UDeathBlowTargetAbility::OnMontageCanceled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UDeathBlowTargetAbility::OnReceiveDeathBlowEvent(FGameplayEventData Payload)
{
	if (DeathBlowEffect)
	{
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(DeathBlowEffect, 1.f);
		ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle);
	}
}

void UDeathBlowTargetAbility::OnReceiveDeathBlowRestoreEvent(FGameplayEventData Payload)
{
	if (DeathBlowRestoreEffect)
	{
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(DeathBlowRestoreEffect, 1.f);
		ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle);
	}
}

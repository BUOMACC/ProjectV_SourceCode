// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathBlowAbility.h"
#include "AbilityTask_ScanDeathBlow.h"

UDeathBlowAbility::UDeathBlowAbility()
{
	// Default Values
	bAutoActivated = true;

	DeathBlowScanRate = 0.1f;
	DeathBlowScanRadius = 300.f;

	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UDeathBlowAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (HasAuthority(&ActivationInfo) == false)
		return;

	UAbilityTask_ScanDeathBlow* ScanTask = UAbilityTask_ScanDeathBlow::ScanDeathBlow(this, DeathBlowScanRate, DeathBlowScanRadius, DeathBlowTraceChannel);
	ScanTask->ReadyForActivation();
}

void UDeathBlowAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

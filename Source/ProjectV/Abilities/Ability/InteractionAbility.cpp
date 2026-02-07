// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionAbility.h"
#include "Abilities/Task/AbilityTask_ScanNearInteractActor.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "CombatGameplayTags.h"
#include "Interface/InteractionInterface.h"

FInteractScannedEvent UInteractionAbility::OnInteractScanned;

UInteractionAbility::UInteractionAbility()
{
	// Tags
	AbilityTags.AddTag(FCombatGameplayTags::Get().Ability_Type_Action_Interact);

	// Default Values
	bAutoActivated = true;

	ScanRadius = 100.f;
	ScanAngle = 80.f;
	ScanInterval = 0.1f;
	bShowScanDebug = false;

	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UInteractionAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_ScanNearInteractActor* ScanTask = UAbilityTask_ScanNearInteractActor::ScanNearInteractActor(this, ScanRadius, ScanAngle, ScanInterval, bShowScanDebug);
	ScanTask->OnScan.AddDynamic(this, &ThisClass::OnScan);
	ScanTask->ReadyForActivation();
}

void UInteractionAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(DelayHandle);
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UInteractionAbility::OnScan(AActor* PrevActor, AActor* NewActor)
{
	CurrentScannedActor = NewActor;

	OnInteractScanned.Broadcast(GetCombatCharacter(), CurrentScannedActor.Get());
}
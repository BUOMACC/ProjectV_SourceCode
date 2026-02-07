// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Ability/ProjectVGameplayAbility.h"
#include "DeathBlowAbility.generated.h"

/**
 *	치명적 일격(인살) 가능한 대상을 탐지하고 어빌리티를 부여합니다.
 */
UCLASS()
class PROJECTV_API UDeathBlowAbility : public UProjectVGameplayAbility
{
	GENERATED_BODY()
	
public:
	UDeathBlowAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DeathBlow Ability")
	float DeathBlowScanRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DeathBlow Ability")
	float DeathBlowScanRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DeathBlow Ability")
	TEnumAsByte<ETraceTypeQuery> DeathBlowTraceChannel;
};

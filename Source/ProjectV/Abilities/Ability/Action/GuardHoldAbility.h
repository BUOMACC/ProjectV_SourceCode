// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Ability/ProjectVGameplayAbility.h"
#include "GuardHoldAbility.generated.h"

/**
 *	가드를 유지하는 동안 실행되는 어빌리티입니다.
 */
UCLASS()
class PROJECTV_API UGuardHoldAbility : public UProjectVGameplayAbility
{
	GENERATED_BODY()

public:
	UGuardHoldAbility();

protected:
	// UGameplayAbility
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	// ~UGameplayAbility

	UFUNCTION()
	void OnInputRelease(float TimeHeld);

	UFUNCTION()
	void OnGuardStopMontageBlendingOut();

	UFUNCTION()
	void OnGuardStopMontageInterrupted();

	UFUNCTION()
	void OnGuardStopMontageCacncelled();

protected:
	UFUNCTION(BlueprintCallable)
	void ForceRelease();

protected:
	/* 가드를 내릴때 실행되는 애니메이션입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Guard Hold Ability")
	TObjectPtr<UAnimMontage> GuardStopMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Guard Hold Ability")
	float PlayRate;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitAbility.h"
#include "GuardBreakHitAbility.generated.h"

/**
 *	가드무시 공격으로 피해를 입었을때에 대한 반응입니다.
 */
UCLASS()
class PROJECTV_API UGuardBreakHitAbility : public UHitAbility
{
	GENERATED_BODY()
	
public:
	UGuardBreakHitAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCanceled();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GuardBreak Ability")
	TObjectPtr<UAnimMontage> GuardBreakMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GuardBreak Ability")
	float PlayRate;
};

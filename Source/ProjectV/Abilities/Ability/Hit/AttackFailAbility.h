// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Ability/Hit/HitAbility.h"
#include "AttackFailAbility.generated.h"

/**
 *	패링등의 이유로 공격이 실패했을때 반응입니다.
 */
UCLASS()
class PROJECTV_API UAttackFailAbility : public UHitAbility
{
	GENERATED_BODY()
	
public:
	UAttackFailAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageCanceled();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack Fail Ability")
	TObjectPtr<UAnimMontage> AttackFailMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack Fail Ability")
	float PlayRate;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitAbility.h"
#include "GuardHitAbility.generated.h"

/**
 *	가드 상태에서 피격시 반응에 대한 어빌리티입니다.
 */
UCLASS()
class PROJECTV_API UGuardHitAbility : public UHitAbility
{
	GENERATED_BODY()
	
public:
	UGuardHitAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCanceled();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Guard Hit Ability")
	TArray<TObjectPtr<UAnimMontage>> GuardHitMontageList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Guard Hit Ability")
	float PlayRate;
};

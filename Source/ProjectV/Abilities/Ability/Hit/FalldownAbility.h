// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitAbility.h"
#include "FalldownAbility.generated.h"

/**
 *	위에서 내려찍어 넘어지는 피격 반응에 대한 어빌리티입니다.
 *	 ex: 내려찍기 등으로 아래를를 향해 넘어질때
 */
UCLASS()
class PROJECTV_API UFalldownAbility : public UHitAbility
{
	GENERATED_BODY()
	
public:
	UFalldownAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCanceled();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Falldown Ability")
	TObjectPtr<UAnimMontage> FalldownMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Falldown Ability")
	float PlayRate;
};

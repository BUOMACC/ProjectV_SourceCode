// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitAbility.h"
#include "KnockdownAbility.generated.h"

/**
 *	밀려나면서 넘어지는 피격 반응에 대한 어빌리티입니다.
 *	 ex: 찌르기 공격 등으로 위를 향해 넘어질때
 */
UCLASS()
class PROJECTV_API UKnockdownAbility : public UHitAbility
{
	GENERATED_BODY()
	
public:
	UKnockdownAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCanceled();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Knockdown Ability")
	TObjectPtr<UAnimMontage> KnockdownMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Knockdown Ability")
	float PlayRate;
};

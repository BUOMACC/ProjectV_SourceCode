// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitAbility.h"
#include "ParrySuccessAbility.generated.h"

/**
 *	패링에 성공했을때 발생하는 반응입니다.
 */
UCLASS()
class PROJECTV_API UParrySuccessAbility : public UHitAbility
{
	GENERATED_BODY()
	
public:
	UParrySuccessAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageBlendOut();

	UFUNCTION()
	void OnMontageCanceled();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parry Success Ability")
	FHitMontageRandomList ParrySuccessMontages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parry Success Ability")
	float PlayRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Parry Success Ability")
	TSubclassOf<UGameplayEffect> DummyEffect;
};

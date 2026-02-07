// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitAbility.h"
#include "SmallHitAbility.generated.h"

/**
 *	약한 피격 반응에 대한 어빌리티입니다.
 *	피격시 넉백과 함께 밀려나는 약경직입니다.
 */
UCLASS()
class PROJECTV_API USmallHitAbility : public UHitAbility
{
	GENERATED_BODY()
	
public:
	USmallHitAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageCanceled();

protected:
	UPROPERTY(EditAnywhere, Category = "Small Hit Ability", meta = (ArraySizeEnum = "EHitDirectionType"))
	FHitMontageSet SmallHitMontages[(int32)EHitDirectionType::End];

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Small Hit Ability")
	float PlayRate;
};

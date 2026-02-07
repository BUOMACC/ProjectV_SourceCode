// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitAbility.h"
#include "LargeHitAbility.generated.h"

/**
 *	강한 피격 반응에 대한 어빌리티입니다.
 *	피격시 RootMotion과 함께 밀려나는 강경직입니다.
 */
UCLASS()
class PROJECTV_API ULargeHitAbility : public UHitAbility
{
	GENERATED_BODY()
	
public:
	ULargeHitAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageCanceled();

protected:
	UPROPERTY(EditAnywhere, Category = "Large Hit Ability", meta = (ArraySizeEnum = "EHitDirectionType"))
	TObjectPtr<UAnimMontage> LargeHitMontageList[(int32)EHitDirectionType::End];

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Large Hit Ability")
	float PlayRate;
};

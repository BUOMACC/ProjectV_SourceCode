// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitAbility.h"
#include "GroundAbility.generated.h"

/**
 *	넘어진 이후 일어나는 연계 동작입니다.
 *	TriggetEventData로 실행되지 않으며, 선행 피격어빌리티 이후에 연결되는 동작입니다.
 * 
 *	누워있는 상태부터 일어서는 동작까지를 내포합니다.
 * 
 *	ex: Airborne -> Ground && Getup
 *		Knockdown -> Ground && Getup
 *		Falldown -> Ground && Getup
 */
UCLASS()
class PROJECTV_API UGroundAbility : public UHitAbility
{
	GENERATED_BODY()
	
public:
	UGroundAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCanceled();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ground Ability")
	TObjectPtr<UAnimMontage> GroundMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ground Ability")
	float PlayRate;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ComboAbility.h"
#include "AirFirstComboAbility.generated.h"

/**
 *	공중 첫 공격에 대한 어빌리티입니다.
 */
UCLASS()
class PROJECTV_API UAirFirstComboAbility : public UComboAbility
{
	GENERATED_BODY()
	
public:
	UAirFirstComboAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageCanceled();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Air First Combo Ability")
	TObjectPtr<UAnimMontage> AirFirstComboMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Air First Combo Ability")
	float PlayRate;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ComboAbility.h"
#include "FirstComboAbility.generated.h"

/**
 *	지상 첫 공격에 대한 어빌리티입니다.
 */
UCLASS()
class PROJECTV_API UFirstComboAbility : public UComboAbility
{
	GENERATED_BODY()
	
public:
	UFirstComboAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageCanceled();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "First Combo Ability")
	TObjectPtr<UAnimMontage> FirstComboMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "First Combo Ability")
	float PlayRate;
};

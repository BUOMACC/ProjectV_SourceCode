// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ComboAbility.h"
#include "LinkComboAbility.generated.h"

/**
 *	지상 첫 공격 이후 연결되는 공격에 대한 어빌리티입니다.
 */
UCLASS()
class PROJECTV_API ULinkComboAbility : public UComboAbility
{
	GENERATED_BODY()
	
public:
	ULinkComboAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageCanceled();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Link Combo Ability")
	TObjectPtr<UAnimMontage> LinkComboMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Link Combo Ability")
	float PlayRate;
};

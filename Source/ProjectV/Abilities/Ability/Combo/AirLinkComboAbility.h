// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ComboAbility.h"
#include "AirLinkComboAbility.generated.h"

/**
 *	피격의 기반이 되는 어빌리티입니다.
 *  피격처리에 필요한 기본적인 데이터와 넉백 기능이 포함되어 있습니다.
 */
UCLASS()
class PROJECTV_API UAirLinkComboAbility : public UComboAbility
{
	GENERATED_BODY()
	
public:
	UAirLinkComboAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageCanceled();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Air Link Combo Ability")
	TObjectPtr<UAnimMontage> AirLinkComboMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Air Link Combo Ability")
	float PlayRate;
};

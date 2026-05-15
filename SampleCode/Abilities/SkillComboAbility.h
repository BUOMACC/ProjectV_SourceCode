// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Ability/Combo/ComboAbility.h"
#include "SkillComboAbility.generated.h"

/**
 *	스킬 공격에 대한 어빌리티입니다.
 */
UCLASS()
class PROJECTV_API USkillComboAbility : public UComboAbility
{
	GENERATED_BODY()
	
public:
	USkillComboAbility();

protected:
	// UGameplayAbility
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	// ~UGameplayAbility

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageCanceled();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "First Combo Ability")
	TObjectPtr<UAnimMontage> SkillMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "First Combo Ability")
	float PlayRate;
};

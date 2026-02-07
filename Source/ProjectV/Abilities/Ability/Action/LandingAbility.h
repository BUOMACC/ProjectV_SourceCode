// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Ability/ProjectVGameplayAbility.h"
#include "LandingAbility.generated.h"

/**
 *	점프 액션 이후 착지하는 행동에 대한 어빌리티입니다.
 *	일반적인 점프 착지에는 적용되지 않습니다.
 *	(ex: 점프 공격 이후 착지)
 */
UCLASS()
class PROJECTV_API ULandingAbility : public UProjectVGameplayAbility
{
	GENERATED_BODY()
	
public:
	ULandingAbility();

protected:
	// UGameplayAbility
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	// ~UGameplayAbility

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageCanceled();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Landing Ability")
	TObjectPtr<UAnimMontage> LandingMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Landing Ability")
	float PlayRate;
};

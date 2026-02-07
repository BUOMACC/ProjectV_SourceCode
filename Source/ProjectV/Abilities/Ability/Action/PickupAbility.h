// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Ability/ProjectVGameplayAbility.h"
#include "PickupAbility.generated.h"

/**
 *	상호작용(아이템을 줍는 행위)에 대한 어빌리티입니다.
 */
UCLASS()
class PROJECTV_API UPickupAbility : public UProjectVGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPickupAbility();

protected:
	// UGameplayAbility
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	// ~UGameplayAbility

	UFUNCTION()
	void OnNetSync();

private:
	void PickupItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup Ability")
	TObjectPtr<UAnimMontage> PickupMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup Ability")
	float PlayRate;
};

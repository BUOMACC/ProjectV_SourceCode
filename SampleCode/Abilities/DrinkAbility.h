// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Ability/ProjectVGameplayAbility.h"
#include "DrinkAbility.generated.h"

class UItemInstance;

/**
 *	물약 등 아이템을 사용하여 마시는 행위에 대한 어빌리티입니다.
 */
UCLASS()
class PROJECTV_API UDrinkAbility : public UProjectVGameplayAbility
{
	GENERATED_BODY()
	
public:
	UDrinkAbility();

protected:
	// UGameplayAbility
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	// ~UGameplayAbility

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageCanceled();

	UFUNCTION()
	void OnReceiveConsumeEvent(FGameplayEventData Payload);

	UFUNCTION()
	void OnReceiveAfterSpawnEvent(FGameplayEventData Payload);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dodge Ability")
	TObjectPtr<UAnimMontage> DrinkMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dodge Ability")
	float PlayRate;

private:
	TWeakObjectPtr<UItemInstance> CachedItemInstance;
};

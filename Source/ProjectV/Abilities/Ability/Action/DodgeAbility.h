// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Ability/ProjectVGameplayAbility.h"
#include "DodgeAbility.generated.h"

/**
 *	구르기 등의 회피하는 동작입니다.
 */
UCLASS()
class PROJECTV_API UDodgeAbility : public UProjectVGameplayAbility
{
	GENERATED_BODY()
	
public:
	UDodgeAbility();

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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dodge Ability")
	TObjectPtr<UAnimMontage> DodgeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dodge Ability")
	float PlayRate;

	/* 회피가 시작되면 실행되는 이펙트입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dodge Ability")
	TSubclassOf<UGameplayEffect> DodgeEffect;
};

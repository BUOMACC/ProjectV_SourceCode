// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitAbility.h"
#include "GroggyAbility.generated.h"

/**
 *	그로기 상태가 되었을때 발생하는 어빌리티입니다.
 */
UCLASS()
class PROJECTV_API UGroggyAbility : public UHitAbility
{
	GENERATED_BODY()
	
public:
	UGroggyAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageCanceled();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Groggy Ability")
	TObjectPtr<UAnimMontage> GroggyMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Groggy Ability")
	float PlayRate;

	/* 그로기가 발동되면 실행되는 이펙트입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Groggy Ability")
	TSubclassOf<UGameplayEffect> GroggyEffect;

	/* 그로기가 종료되면 실행되는 이펙트입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Groggy Ability")
	TSubclassOf<UGameplayEffect> GroggyEndEffect;
};

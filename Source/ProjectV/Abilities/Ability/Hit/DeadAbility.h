// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Ability/Hit/HitAbility.h"
#include "DeadAbility.generated.h"

/**
 *	죽었을때 활성화되는 어빌리티입니다.
 */
UCLASS()
class PROJECTV_API UDeadAbility : public UHitAbility
{
	GENERATED_BODY()
	
public:
	UDeadAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Dead Ability")
	TArray<TObjectPtr<UAnimMontage>> DeadMontageList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dead Ability")
	float PlayRate;
};

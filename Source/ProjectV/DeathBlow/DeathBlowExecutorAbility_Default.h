// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DeathBlow/DeathBlowExecutorAbility.h"
#include "DeathBlowExecutorAbility_Default.generated.h"


UCLASS()
class PROJECTV_API UDeathBlowExecutorAbility_Default : public UDeathBlowExecutorAbility
{
	GENERATED_BODY()
	
protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
};

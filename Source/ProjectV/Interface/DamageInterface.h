// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageInterface.generated.h"

struct FGameplayEffectContextHandle;
class UHitData;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *	피해를 입을 대상에게 구현하여 사용
 */
class PROJECTV_API IDamageInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Damage(UAbilitySystemComponent* AttackerASC, const FGameplayAbilityTargetDataHandle& TargetDataHandle, UHitData* HitData);
};

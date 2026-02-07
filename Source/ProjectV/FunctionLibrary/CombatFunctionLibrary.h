// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Abilities/CombatAbilitySystemComponent.h"
#include "Abilities/CombatGameplayEffectContext.h"
#include "CombatFunctionLibrary.generated.h"

class UHitData;
class UGameplayEffect;

/*
 *	Combat Helper Functions for Blueprint.
 *	 e.g) 대상에게 데미지를 입히는 등 전투와 관련된 도움 함수들이 내장됨
 */
UCLASS()
class PROJECTV_API UCombatFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "CombatAbility FunctionLibrary")
	static void TestFunc();
};

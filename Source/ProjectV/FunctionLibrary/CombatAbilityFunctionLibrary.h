// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Abilities/CombatAbilitySystemComponent.h"
#include "Abilities/CombatGameplayEffectContext.h"
#include "Abilities/Ability/ProjectVGameplayAbility.h"
#include "CombatAbilityFunctionLibrary.generated.h"

class UHitData;
class UGameplayEffect;

/*
 *	Ability Helper Functions for Blueprint.
 */
UCLASS()
class PROJECTV_API UCombatAbilityFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "CombatAbility FunctionLibrary")
	static void EffectContextAddHitData(FGameplayEffectContextHandle Handle, UHitData* HitData);
	
	UFUNCTION(BlueprintPure, Category = "CombatAbility FunctionLibrary")
	static UHitData* EffectContextGetHitData(const FGameplayEffectContextHandle& Handle);

	/*
	*	HitData를 포함한 DamageSpec을 생성하고 필요 데이터(SetByCaller)를 설정합니다.
	*	<설정되는 데이터>
	*	 - FixedDamage
	*	 - WeaponDamage
	*	 - GroggyGauge
	*	 - ParryGauge
	*/
	
	UFUNCTION(BlueprintPure, Category = "CombatAbility FunctionLibrary")
	static FGameplayEffectSpecHandle GenerateDamageSpecFromHitData(TSubclassOf<UGameplayEffect> DamageClass, UHitData* HitData, UAbilitySystemComponent* InstigatorASC);

	UFUNCTION(BlueprintPure, Category = "CombatAbility FunctionLibrary")
	static FHitResult GetFirstHitResultFromTargetData(const FGameplayAbilityTargetDataHandle& TargetData);
};

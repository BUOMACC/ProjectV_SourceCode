// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "DamageEffectSelectorBase.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;
class UHitData;

/**
 *	HitMainType Tag값에 따라 원하는 Damage Effect를 선택할 수 있도록 가공해주는 클래스
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class PROJECTV_API UDamageEffectSelectorBase : public UObject
{
	GENERATED_BODY()
	
public:
	UDamageEffectSelectorBase();

	/*
	*	입력받은 대상에게 입혀야 하는 적절한 데미지 클래스를 선택합니다.
	*	 @param DamageInstigator 데미지를 가할때 이를 책임지는 실제 공격자입니다.
	*	 @param DamageCauser 데미지를 가한 객체입니다. (e.g 투사체)
	*	 @param Target 데미지를 가할 대상입니다.
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage Effect Selector")
	TSubclassOf<UGameplayEffect> ChooseDamageEffectClass(FGameplayTag HitMainType, FGameplayTag HitSubType, bool bCanParry, AActor* DamageInstigator, AActor* DamageCauser, AActor* Target) const;
	virtual TSubclassOf<UGameplayEffect> ChooseDamageEffectClass_Implementation(FGameplayTag HitMainType, FGameplayTag HitSubType, bool bCanParry, AActor* DamageInstigator, AActor* DamageCauser, AActor* Target) const;
};

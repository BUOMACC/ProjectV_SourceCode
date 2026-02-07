// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "GroggyGaugeMMC.generated.h"

/**
 *	체력이 절반 아래인 경우 그로기(체간) 회복량을 75% 감소시키기 위한 연산 클래스입니다.
 * 
 *	- 체력이 절반 이상인 경우 1을 반환
 *	- 체력이 절반 미만인 경우 0.25를 반환
 */
UCLASS()
class PROJECTV_API UGroggyGaugeMMC : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	UGroggyGaugeMMC();

protected:
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition HealthDef;
	FGameplayEffectAttributeCaptureDefinition MaxHealthDef;
};

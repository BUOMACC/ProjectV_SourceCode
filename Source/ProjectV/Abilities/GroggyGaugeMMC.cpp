// Fill out your copyright notice in the Description page of Project Settings.


#include "GroggyGaugeMMC.h"
#include "AttributeSet/HealthAttributeSet.h"

UGroggyGaugeMMC::UGroggyGaugeMMC()
{
	HealthDef.AttributeToCapture = UHealthAttributeSet::GetHealthAttribute();
	HealthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	HealthDef.bSnapshot = false;

	MaxHealthDef.AttributeToCapture = UHealthAttributeSet::GetHealthAttribute();
	MaxHealthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	MaxHealthDef.bSnapshot = true; // 변경될 여지가 없으므로, 스냅샷을 켜서 비용줄임

	RelevantAttributesToCapture.Add(HealthDef);
	RelevantAttributesToCapture.Add(MaxHealthDef);
}

float UGroggyGaugeMMC::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// 체력을 가져오기 위한 사전 작업
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// 체력 가져온 뒤 계산
	float Health = 0.f;
	GetCapturedAttributeMagnitude(HealthDef, Spec, EvaluationParameters, Health);

	float MaxHealth = 0.f;
	GetCapturedAttributeMagnitude(MaxHealthDef, Spec, EvaluationParameters, MaxHealth);
	MaxHealth = FMath::Max<float>(MaxHealth, 1.f); // 0 나누기 방지

	float HealthPercent = (Health / MaxHealth);
	if (HealthPercent < 0.5f)
	{
		return 0.25f;
	}

	return 1.0f;
}

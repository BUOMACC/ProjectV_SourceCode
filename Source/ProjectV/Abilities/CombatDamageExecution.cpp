// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatDamageExecution.h"
#include "CombatGameplayTags.h"
#include "AttributeSet/HealthAttributeSet.h"
#include "AttributeSet/CombatAttributeSet.h"

UCombatDamageExecution::UCombatDamageExecution()
{

}

void UCombatDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
#if WITH_SERVER_CODE
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContext* Context = Spec.GetContext().Get();
	check(Context);

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	FGameplayTagContainer DamageAssetTag;
	Spec.GetAllAssetTags(DamageAssetTag);

	const FCombatGameplayTags& CombatTags = FCombatGameplayTags::Get();

	// Calculate Weapon Damage
	float WeaponDamage = 0.0f;
	WeaponDamage += Spec.GetSetByCallerMagnitude(CombatTags.SetByCaller_WeaponDamage, false, 0.f);

	// Calculate Fixed Damage
	float FixedDamage = 0.0f;
	FixedDamage += Spec.GetSetByCallerMagnitude(CombatTags.SetByCaller_FixedDamage, false, 0.f);

	float TotalDamage = FMath::Max(WeaponDamage + FixedDamage, 1.0f);

	// Calculate Guard
	if (DamageAssetTag.HasTagExact(CombatTags.HitEvent_Result_Guard))
	{
		// 90% Decrease Total Damage
		TotalDamage *= 0.1f;
	}

	// if Target has One Damage Cheat, Set Damage to 1
	if (TargetTags->HasTagExact(CombatTags.Character_State_OneDamage))
	{
		TotalDamage = 1.f;
	}

	// if Parry, Set Damage to 0
	if (DamageAssetTag.HasTagExact(CombatTags.HitEvent_Result_Parry))
	{
		TotalDamage = 0.f;
	}

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UHealthAttributeSet::GetReceivedDamageAttribute(), EGameplayModOp::Additive, TotalDamage));
#endif
}

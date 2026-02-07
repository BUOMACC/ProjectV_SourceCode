// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatAttributeSet.h"
#include "CombatGameplayTags.h"
#include "Abilities/CombatAbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffectTypes.h"
#include "Net/UnrealNetwork.h"

UCombatAttributeSet::UCombatAttributeSet()
{
	// Stamina
	Stamina = 100.f;
	MaxStamina = 100.f;
	
	// Guard Angle - (e.g: 90 = left + right)
	GuardAngle = 90.f;

	// Groggy - (if GroggyGauge > MaxGroggyGauge -> Groggy)
	GroggyGauge = 0.f;
	MaxGroggyGauge = 100.f;
}

void UCombatAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatAttributeSet, Stamina);
	DOREPLIFETIME(UCombatAttributeSet, MaxStamina);
	DOREPLIFETIME(UCombatAttributeSet, GuardAngle);
	DOREPLIFETIME(UCombatAttributeSet, GroggyGauge);
	DOREPLIFETIME(UCombatAttributeSet, MaxGroggyGauge);
}

bool UCombatAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	// 그로기 게이지만 무적판정에 적용
	if (Data.EvaluatedData.Attribute == GetGroggyGaugeAttribute())
	{
		const FCombatGameplayTags& CombatTags = FCombatGameplayTags::Get();

		FGameplayTagContainer DamageEffectTagContainer;
		Data.EffectSpec.GetAllAssetTags(DamageEffectTagContainer);

		// if Dead? -> Ignore All Gameplay Effect
		if (Data.Target.HasMatchingGameplayTag(CombatTags.Character_MainState_Dead))
		{
			Data.EvaluatedData.Magnitude = 0.f;
			return false;
		}

		// if Invincible? -> Only Allow System Damage
		if (Data.Target.HasMatchingGameplayTag(CombatTags.Character_State_Invincible)
			&& DamageEffectTagContainer.HasTagExact(CombatTags.HitEvent_Result_System) == false)
		{
			Data.EvaluatedData.Magnitude = 0.f;
			return false;
		}
	}

    return true;
}

void UCombatAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

}

void UCombatAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// CurrentValue가 변경될시 Clamping 한다.
	ClampValue(Attribute, NewValue);
}

void UCombatAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void UCombatAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	// BaseValue가 변경될시 Clamping 한다.
	// Regen같은 경우, Periodic 타입은 Instant(BaseValue)로 적용되므로, 이곳에서도 클램핑 작업이 이루어져야 한다.
	ClampValue(Attribute, NewValue);
}

void UCombatAttributeSet::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
}

void UCombatAttributeSet::ClampValue(const FGameplayAttribute& Attribute, float& Value /* 변경될 값임 (덮어씌워질 값) */) const
{
	if (Attribute == GetStaminaAttribute())
	{
		Value = FMath::Clamp(Value, 0.f, GetMaxStamina());
	}
	else if (Attribute == GetGuardAngleAttribute())
	{
		Value = FMath::Clamp(Value, 0.f, 270.f);
	}
	else if (Attribute == GetGroggyGaugeAttribute())
	{
		Value = FMath::Clamp(Value, 0.f, GetMaxGroggyGauge());
	}
}

void UCombatAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, Stamina, OldValue);
}

void UCombatAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, MaxStamina, OldValue);
}

void UCombatAttributeSet::OnRep_GuardAngle(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, GuardAngle, OldValue);
}

void UCombatAttributeSet::OnRep_GroggyGauge(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, GroggyGauge, OldValue);
}

void UCombatAttributeSet::OnRep_MaxGroggyGauge(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, MaxGroggyGauge, OldValue);
}
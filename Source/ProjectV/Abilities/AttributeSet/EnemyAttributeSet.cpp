// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAttributeSet.h"
#include "Abilities/CombatAbilitySystemComponent.h"
#include "CombatGameplayTags.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffectTypes.h"
#include "Net/UnrealNetwork.h"

UEnemyAttributeSet::UEnemyAttributeSet()
{
	ParryGauge = 100.f;
	Life = 1;
	MaxLife = 1;
}

void UEnemyAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEnemyAttributeSet, Life);
	DOREPLIFETIME(UEnemyAttributeSet, MaxLife);
}

void UEnemyAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// CurrentValue가 변경될시 Clamping 한다.
	ClampValue(Attribute, NewValue);
}

void UEnemyAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

bool UEnemyAttributeSet::PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	if (Data.EvaluatedData.Attribute == GetParryGaugeAttribute())
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

void UEnemyAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void UEnemyAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	// BaseValue가 변경될시 Clamping 한다.
	// Regen같은 경우, Periodic 타입은 Instant(BaseValue)로 적용되므로, 이곳에서도 클램핑 작업이 이루어져야 한다.
	ClampValue(Attribute, NewValue);
}

void UEnemyAttributeSet::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
}

void UEnemyAttributeSet::ClampValue(const FGameplayAttribute& Attribute, float& Value /* 변경될 값임 (덮어씌워질 값) */) const
{
	if (Attribute == GetParryGaugeAttribute())
	{
		Value = FMath::Clamp(Value, 0.f, Value);
	}
	if (Attribute == GetLifeAttribute())
	{
		Value = FMath::Clamp(Value, 0.f, GetMaxLife());
	}
}

void UEnemyAttributeSet::OnRep_Life(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEnemyAttributeSet, Life, OldValue);
}

void UEnemyAttributeSet::OnRep_MaxLife(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEnemyAttributeSet, MaxLife, OldValue);
}
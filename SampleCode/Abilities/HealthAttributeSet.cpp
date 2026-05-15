// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthAttributeSet.h"
#include "Abilities/CombatAbilitySystemComponent.h"
#include "CombatGameplayTags.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffectTypes.h"
#include "Net/UnrealNetwork.h"

UHealthAttributeSet::UHealthAttributeSet()
{
	Health = 100.f;
	MaxHealth = 100.f;
}

void UHealthAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthAttributeSet, Health);
	DOREPLIFETIME(UHealthAttributeSet, MaxHealth);
}

void UHealthAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampValue(Attribute, NewValue);
}

void UHealthAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

bool UHealthAttributeSet::PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

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

	return true;
}

void UHealthAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetReceivedDamageAttribute())
	{
		const FGameplayEffectContextHandle& Context = Data.EffectSpec.GetEffectContext();
		float DamageToTaken = Data.EvaluatedData.Magnitude;
		float RemainHealth = FMath::Clamp(GetHealth() - DamageToTaken, 0.f, GetMaxHealth());

		SetHealth(RemainHealth);
		SetReceivedDamage(0.f);

		AActor* DamageInstigator = Context.GetInstigator();
		// AActor* DamageCauser = Context.GetEffectCauser();	// same as Instigator(because AvatarActor == OwnerActor)
		AActor* SourceObjectActor = nullptr;					// TODO: if you needed, add projectile actor here.
		if (UObject* SourceObject = Context.GetSourceObject())
		{
			SourceObjectActor = Cast<AActor>(SourceObject);
		}

		// Damage Event
		if (OnDamaged.IsBound())
		{
			OnDamaged.Broadcast(DamageInstigator, SourceObjectActor, Data.EffectSpec, DamageToTaken);
		}

		// OutOfHealth Event if Health <= 0
		if (GetHealth() <= 0.f)
		{
			if (OnOutOfHealth.IsBound())
			{
				OnOutOfHealth.Broadcast(DamageInstigator, SourceObjectActor, Data.EffectSpec, DamageToTaken);
			}
		}
	}
}

void UHealthAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampValue(Attribute, NewValue);
}

void UHealthAttributeSet::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
}

void UHealthAttributeSet::ClampValue(const FGameplayAttribute& Attribute, float& Value) const
{
	if (Attribute == GetHealthAttribute())
	{
		Value = FMath::Clamp(Value, 0.f, GetMaxHealth());
	}
}

void UHealthAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, Health, OldValue);
}

void UHealthAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, MaxHealth, OldValue);
}
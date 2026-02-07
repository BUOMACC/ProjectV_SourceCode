// Fill out your copyright notice in the Description page of Project Settings.


#include "GrantAbilityData.h"
#include "Abilities/Ability/ActionAbility.h"
#include "Abilities/CombatAbilitySystemComponent.h"
#include "CombatGameplayTags.h"

void FGrantedAbilityHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FGrantedAbilityHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayEffectHandles.Add(Handle);
	}
}

void FGrantedAbilityHandles::TakeFromAbilitySystem(UAbilitySystemComponent* CombatASC)
{
	check(CombatASC);

	if (!CombatASC->IsOwnerActorAuthoritative())
	{
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			CombatASC->ClearAbility(Handle);
		}
	}

	for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		if (Handle.IsValid())
		{
			CombatASC->RemoveActiveGameplayEffect(Handle);
		}
	}

	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
}

bool FGrantedAbilityHandles::CanRemoveNow(UAbilitySystemComponent* CombatASC) const
{
	check(CombatASC);

	if (AbilityTakeRule == EGrantedAbilityRule::Pending)
	{
		for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
		{
			const FGameplayAbilitySpec* Spec = CombatASC->FindAbilitySpecFromHandle(Handle);
			if (Spec && Spec->IsActive())
			{
				return false;
			}
		}
	}

	return true;
}


UGrantAbilityData::UGrantAbilityData()
	: TypeTag(FGameplayTag::EmptyTag),
	  AbilityTakeRule(EGrantedAbilityRule::Instant),
	  GrantAbilities(TArray<TSubclassOf<UGameplayAbility>>()),
	  GrantGameplayEffects(TArray<TSubclassOf<UGameplayEffect>>())
{
	// ...
}
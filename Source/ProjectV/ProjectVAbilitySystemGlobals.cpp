// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectVAbilitySystemGlobals.h"
#include "Abilities/CombatGameplayEffectContext.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ProjectVAbilitySystemGlobals)

struct FGameplayEffectContext;

UProjectVAbilitySystemGlobals::UProjectVAbilitySystemGlobals(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FGameplayEffectContext* UProjectVAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FCombatGameplayEffectContext();
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatPlayerState.h"
#include "Abilities/CombatAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

UAbilitySystemComponent* ACombatPlayerState::GetAbilitySystemComponent() const
{
	if (GetPawn())
	{
		return UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn());
	}
	return nullptr;
}
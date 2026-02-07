// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectVAssetManager.h"
#include "CombatGameplayTags.h"
#include "AbilitySystemGlobals.h"

void UProjectVAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FCombatGameplayTags::Get().InitializeGameplayTags();
	UAbilitySystemGlobals::Get().InitGlobalData();
}
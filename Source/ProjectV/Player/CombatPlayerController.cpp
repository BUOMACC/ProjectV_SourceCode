// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatPlayerController.h"
#include "CombatGameplayTags.h"
#include "Kismet/KismetMathLibrary.h"
#include "Setting/ProjectVUserSettingsShared.h"
#include "UI/CombatHUD.h"

void ACombatPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ACombatPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PostProcessInput(DeltaTime, bGamePaused);

	if (UCombatAbilitySystemComponent* CombatASC = GetCombatAbilitySystemComponent())
	{
		CombatASC->ProcessAbilityInput();
	}
}

void ACombatPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void ACombatPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

UCombatAbilitySystemComponent* ACombatPlayerController::GetCombatAbilitySystemComponent() const
{
	if (ACombatCharacter* ControlledCharacter = GetControlledCharacter())
	{
		return ControlledCharacter->GetCombatAbilitySystemComponent();
	}
	return nullptr;
}

ACombatCharacter* ACombatPlayerController::GetControlledCharacter() const
{
	if (ACombatCharacter* ControlledCharacter = Cast<ACombatCharacter>(GetCharacter()))
	{
		return ControlledCharacter;
	}
	return nullptr;
}

void ACombatPlayerController::AcknowledgePossession(class APawn* P)
{
	Super::AcknowledgePossession(P);

	// Add Default Widget
	OnAfterPossessClient();
}

void ACombatPlayerController::SetPlayer(UPlayer* InPlayer)
{
	Super::SetPlayer(InPlayer);
	
	// First Time Load
	GetSharedSettings();
}

UProjectVUserSettingsShared* ACombatPlayerController::GetSharedSettings()
{
	if (SharedSettings == nullptr)
	{
		SharedSettings = UProjectVUserSettingsShared::LoadOrCreateSettings();
	}
	return SharedSettings;
}

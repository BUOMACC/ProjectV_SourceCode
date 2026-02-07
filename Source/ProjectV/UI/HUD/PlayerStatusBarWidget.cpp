// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatusBarWidget.h"
#include "AbilitySystemGlobals.h"
#include "Character/CombatCharacter.h"
#include "GameFramework/PlayerController.h"

#include "Components/ProgressBar.h"

UPlayerStatusBarWidget::UPlayerStatusBarWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UPlayerStatusBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (APlayerController* PC = GetOwningPlayer())
	{
		PC->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPawnChanged);
		OnPawnChanged(nullptr, PC->GetPawn());
	}
}

void UPlayerStatusBarWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (APlayerController* PC = GetOwningPlayer())
	{
		PC->OnPossessedPawnChanged.RemoveDynamic(this, &ThisClass::OnPawnChanged);
	}
}

void UPlayerStatusBarWidget::OnPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	InitializeStatusBar(NewPawn);
}
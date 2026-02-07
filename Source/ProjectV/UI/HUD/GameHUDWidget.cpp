// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHUDWidget.h"
#include "GameFramework/HUD.h"
#include "Interface/CombatUILayerInterface.h"
#include "Input/CommonUIInputTypes.h"
#include "CombatGameplayTags.h"
#include "ProjectVLogs.h"

UGameHUDWidget::UGameHUDWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	BackAction = nullptr;
	BackWidget = nullptr;
}

void UGameHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	RegisterUIActionBinding(FBindUIActionArgs(BackAction, FSimpleDelegate::CreateUObject(this, &UGameHUDWidget::HandlePressBack)));
}

void UGameHUDWidget::HandlePressBack()
{
	if (BackWidget == nullptr)
	{
		UE_LOG(LogProjectV, Warning, TEXT("UGameHUDWidget::HandlePressBack() - Back Widget is nullptr."));
		return;
	}

	if (AHUD* OwningHUD = GetOwningPlayer()->GetHUD())
	{
		if (OwningHUD->Implements<UCombatUILayerInterface>())
		{
			ICombatUILayerInterface::Execute_AddWidget(OwningHUD, FCombatGameplayTags::Get().UI_Layer_Game, BackWidget);
		}
	}
}

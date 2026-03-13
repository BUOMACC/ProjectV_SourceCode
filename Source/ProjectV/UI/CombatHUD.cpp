// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatHUD.h"
#include "CommonActivatableWidget.h"
#include "CombatUILayerWidget.h"
#include "ProjectVLogs.h"

ACombatHUD::ACombatHUD()
{

}

void ACombatHUD::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ACombatHUD::BeginPlay()
{
	Super::BeginPlay();

	if (DefaultLayerWidget)
	{
		LayerWidgetInstance = CreateWidget<UCombatUILayerWidget>(GetWorld(), DefaultLayerWidget);
		LayerWidgetInstance->AddToViewport();
	}
	else
	{
		UE_LOG(LogCombatUI, Warning, TEXT("Can't Regist Default Layer Widget (DefaultLayerWidget is nullptr)."));
	}
}

void ACombatHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

UCommonActivatableWidget* ACombatHUD::AddWidget_Implementation(FGameplayTag InLayerTag, TSubclassOf<UCommonActivatableWidget> WidgetToAdd)
{
	if (LayerWidgetInstance)
	{
		return LayerWidgetInstance->AddWidget(InLayerTag, WidgetToAdd);
	}
	return nullptr;
}

void ACombatHUD::ClearWidgets_Implementation(FGameplayTag InLayerTag)
{
	if (LayerWidgetInstance)
	{
		LayerWidgetInstance->ClearWidgets(InLayerTag);
	}
}

void ACombatHUD::RemoveWidget_Implementation(UCommonActivatableWidget* WidgetToRemove)
{
	if (LayerWidgetInstance)
	{
		LayerWidgetInstance->RemoveWidget(WidgetToRemove);
	}
}

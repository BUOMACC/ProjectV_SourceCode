// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatUILayerWidget.h"
#include "CommonActivatableWidget.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "ProjectVLogs.h"

#define COMBATUI_LAYER_LOG(Verbosity, Format, ...) UE_LOG(LogCombatUI, Verbosity, TEXT("[CombatUILayerWidget] - %s"), *FString::Printf(Format, ##__VA_ARGS__))

void UCombatUILayerWidget::RegistWidgetStack(FGameplayTag InLayerTag, UCommonActivatableWidgetStack* InWidgetStack)
{
	if (InWidgetStack == nullptr)
	{
		COMBATUI_LAYER_LOG(Warning, TEXT("Failed to Regist Widget (WidgetStack is nullptr)."));
		return;
	}

	if (FindWidgetStack(InLayerTag))
	{
		COMBATUI_LAYER_LOG(Warning, TEXT("Failed to Regist Widget (WidgetStack already Registered)."));
		return;
	}

	FRegisteredWidgetLayerInfo LayerInfo;
	LayerInfo.LayerTag = InLayerTag;
	LayerInfo.WidgetStack = InWidgetStack;
	RegisteredInfo.Add(LayerInfo);

	COMBATUI_LAYER_LOG(Log, TEXT("Layer Regist Success. (Layer: %s)"), *InLayerTag.ToString());
}

UCommonActivatableWidget* UCombatUILayerWidget::AddWidget(FGameplayTag& InLayerTag, TSubclassOf<UCommonActivatableWidget> InWidgetToAdd)
{
	check(InWidgetToAdd);
	if (UCommonActivatableWidgetStack* WidgetStack = FindWidgetStack(InLayerTag))
	{
		return WidgetStack->AddWidget(InWidgetToAdd);
	}
	return nullptr;
}

void UCombatUILayerWidget::ClearWidgets(FGameplayTag& InLayerTag)
{
	if (UCommonActivatableWidgetStack* WidgetStack = FindWidgetStack(InLayerTag))
	{
		WidgetStack->ClearWidgets();
	}
}

void UCombatUILayerWidget::RemoveWidget(UCommonActivatableWidget* InWidgetToRemove)
{
	check(InWidgetToRemove);
	InWidgetToRemove->DeactivateWidget();
}

UCommonActivatableWidgetStack* UCombatUILayerWidget::FindWidgetStack(FGameplayTag& InLayerTag)
{
	for (const FRegisteredWidgetLayerInfo& Info : RegisteredInfo)
	{
		if (Info.LayerTag == InLayerTag)
		{
			return Info.WidgetStack.Get();
		}
	}
	return nullptr;
}
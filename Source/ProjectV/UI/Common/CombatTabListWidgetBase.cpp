// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatTabListWidgetBase.h"
#include "CombatTabButtonBase.h"

void UCombatTabListWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ButtonTextMap.Empty();
}

void UCombatTabListWidgetBase::HandleTabCreation_Implementation(FName TabNameID, UCommonButtonBase* TabButton)
{
	if (UCombatTabButtonBase* TabButtonBase = Cast<UCombatTabButtonBase>(TabButton))
	{
		if (ButtonTextMap.Contains(TabNameID))
		{
			TabButtonBase->SetButtonText(ButtonTextMap[TabNameID]);
		}
	}
}

bool UCombatTabListWidgetBase::RegisterTabWithText(FName TabNameID, TSubclassOf<UCommonButtonBase> ButtonWidgetType, FText TabButtonText, UWidget* ContentWidget, const int32 TabIndex)
{
	ButtonTextMap.Add(TabNameID, TabButtonText);
	return RegisterTab(TabNameID, ButtonWidgetType, ContentWidget, TabIndex);
}

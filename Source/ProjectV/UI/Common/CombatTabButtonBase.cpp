// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatTabButtonBase.h"
#include "CommonTextBlock.h"

void UCombatTabButtonBase::SetButtonText(FText& InText)
{
	if (Text_Name)
	{
		Text_Name->SetText(InText);
	}
}

void UCombatTabButtonBase::NativeOnCurrentTextStyleChanged()
{
	Super::NativeOnCurrentTextStyleChanged();

	if (Text_Name == nullptr)
		return;

	if (UCommonButtonStyle* ButtonStyle = GetStyle())
	{
		if (GetSelected())
		{
			Text_Name->SetStyle(ButtonStyle->SelectedTextStyle);
		}
		else
		{
			Text_Name->SetStyle(ButtonStyle->NormalTextStyle);
		}
	}
}

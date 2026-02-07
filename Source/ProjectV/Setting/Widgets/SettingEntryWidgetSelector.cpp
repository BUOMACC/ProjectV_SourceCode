// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingEntryWidgetSelector.h"
#include "Setting/UserSettingSelector.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/KismetStringLibrary.h"

void USettingEntryWidgetSelector::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button_OptionNext->OnClicked.AddDynamic(this, &ThisClass::SelectOptionNext);
	Button_OptionPrev->OnClicked.AddDynamic(this, &ThisClass::SelectOptionPrev);
}

void USettingEntryWidgetSelector::HandleSettingChanged()
{
	Super::HandleSettingChanged();

	if (Setting)
	{
		UUserSettingSelector* SelectorSetting = Cast<UUserSettingSelector>(Setting);
		check(SelectorSetting);

		InitWidget(SelectorSetting->GetDisplayName(), SelectorSetting->GetOptions());
		SelectOption(SelectorSetting->GetValue());
	}

	RefreshWidget();
}

void USettingEntryWidgetSelector::ConfirmSettingValue()
{
	if (Setting)
	{
		UUserSettingSelector* SelectorSetting = Cast<UUserSettingSelector>(Setting);
		SelectorSetting->SetValue(SelectedIndex);
		SelectorSetting->ConfirmValue();
	}
}

void USettingEntryWidgetSelector::InitWidget(FText InSettingName, const TArray<FText>& InOptions)
{
	SettingName = InSettingName;

	for (const FText& OptionText : InOptions)
	{
		AddOption(OptionText, false);
	}

	RefreshWidget();
}

void USettingEntryWidgetSelector::RefreshWidget()
{
	Text_SettingName->SetText(SettingName);

	if (IsValidOptionIndex(SelectedIndex))
	{
		Text_OptionValue->SetText(OptionTexts[SelectedIndex]);
	}
}

void USettingEntryWidgetSelector::AddOption(FText InOptionText, bool bWithRefreshWidget)
{
	OptionTexts.Add(InOptionText);

	if (bWithRefreshWidget)
	{
		RefreshWidget();
	}
}

void USettingEntryWidgetSelector::SelectOption(int32 NewIndex)
{
	if (IsValidOptionIndex(NewIndex))
	{
		SelectedIndex = NewIndex;
	}
	else
	{
		SelectedIndex = 0;
	}

	RefreshWidget();
}

void USettingEntryWidgetSelector::SelectOptionNext()
{
	int32 NextIndex = (SelectedIndex + 1) % GetOptionCount();
	SelectedIndex = NextIndex;

	RefreshWidget();
}

void USettingEntryWidgetSelector::SelectOptionPrev()
{
	int32 PrevIndex = SelectedIndex - 1;
	if (PrevIndex < 0)
	{
		PrevIndex += GetOptionCount();
	}

	SelectedIndex = PrevIndex;

	RefreshWidget();
}

bool USettingEntryWidgetSelector::IsValidOptionIndex(int32 IndexToSelect) const
{
	return OptionTexts.IsValidIndex(SelectedIndex);
}

int32 USettingEntryWidgetSelector::GetOptionCount() const
{
	return OptionTexts.Num();
}


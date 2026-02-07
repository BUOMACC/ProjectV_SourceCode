// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingEntryWidgetSlider.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
#include "Components/Slider.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "SettingEntryWidgetSelector.h"
#include "Setting/UserSettingSlider.h"

void USettingEntryWidgetSlider::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Slider_Value->OnValueChanged.AddDynamic(this, &ThisClass::HandleSliderValueChanged);
	
	ETB_Value->OnTextChanged.AddDynamic(this, &ThisClass::HandleEditableTextChanged);
	ETB_Value->OnTextCommitted.AddDynamic(this, &ThisClass::HandleEditableTextCommitted);
}

void USettingEntryWidgetSlider::HandleSettingChanged()
{
	Super::HandleSettingChanged();

	if (Setting)
	{
		UUserSettingSlider* SliderSetting = Cast<UUserSettingSlider>(Setting);
		check(SliderSetting);

		InitWidget(SliderSetting->GetDisplayName(), SliderSetting->GetValueRange().Key, SliderSetting->GetValueRange().Value, SliderSetting->GetSnapSize());
		SetValue(SliderSetting->GetValue());
	}

	RefreshWidget();
}

void USettingEntryWidgetSlider::ConfirmSettingValue()
{
	if (Setting)
	{
		UUserSettingSlider* SliderSetting = Cast<UUserSettingSlider>(Setting);
		SliderSetting->SetValue(CurrentValue);
		SliderSetting->ConfirmValue();
	}
}

void USettingEntryWidgetSlider::InitWidget(FText InSettingName, float InMinValue, float InMaxValue, float InSnapSize)
{
	SettingName = InSettingName;

	if (InMinValue > InMaxValue)
	{
		MinValue = InMaxValue;
		MaxValue = InMinValue;
	}
	else
	{
		MinValue = InMinValue;
		MaxValue = InMaxValue;
	}

	Slider_Value->SetMinValue(MinValue);
	Slider_Value->SetMaxValue(MaxValue);

	SnapSize = InSnapSize;

	CurrentValue = MinValue;


	RefreshWidget();
}

void USettingEntryWidgetSlider::RefreshWidget()
{
	Slider_Value->SetValue(CurrentValue);
	ETB_Value->SetText(UKismetTextLibrary::Conv_DoubleToText(CurrentValue, ERoundingMode::HalfToEven));
	Text_SettingName->SetText(SettingName);
}

void USettingEntryWidgetSlider::SetValue(float NewValue)
{
	CurrentValue = FMath::Clamp(NewValue, MinValue, MaxValue);
	CurrentValue = FMath::GridSnap(CurrentValue, SnapSize);
}

bool USettingEntryWidgetSlider::CanValueChange(float Value) const
{
	if (Value >= MinValue && Value <= MaxValue)
	{
		return true;
	}

	return false;
}

void USettingEntryWidgetSlider::HandleSliderValueChanged(float Value)
{
	if (CanValueChange(Value))
	{
		SetValue(Value);
		RefreshWidget();
	}
}

void USettingEntryWidgetSlider::HandleEditableTextChanged(const FText& Text)
{
	if (!Text.IsNumeric())
	{
		ETB_Value->SetText(FText::GetEmpty());
	}
}

void USettingEntryWidgetSlider::HandleEditableTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (Text.IsNumeric())
	{
		SetValue(UKismetStringLibrary::Conv_StringToFloat(Text.ToString()));
		RefreshWidget();
	}
	else
	{
		ETB_Value->SetText(FText::GetEmpty());
	}
}
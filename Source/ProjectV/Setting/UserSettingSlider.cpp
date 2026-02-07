// Fill out your copyright notice in the Description page of Project Settings.


#include "UserSettingSlider.h"
#include "UserSettingSelector.h"

UUserSettingSlider::UUserSettingSlider()
{
	Value = 0.f;

	MinValue = 0.f;
	MaxValue = 1.f;

	SnapSize = 0.02f;
}

void UUserSettingSlider::Initialize()
{
	Super::Initialize();
}

void UUserSettingSlider::ConfirmValue()
{
	Setter.ExecuteIfBound(Value);
	SetValue(Value);
}

void UUserSettingSlider::SetValueRange(float NewMinValue, float NewMaxValue)
{
	if (NewMinValue > NewMaxValue)
	{
		MinValue = NewMaxValue;
		MaxValue = NewMinValue;
	}

	MinValue = NewMinValue;
	MaxValue = NewMaxValue;
}

void UUserSettingSlider::BindFunction(FSliderSettingSetterDelegate InSetter, FSliderSettingGetterDelegate InGetter)
{
	Setter = InSetter;
	Getter = InGetter;

	if (Getter.IsBound())
	{
		SetValue(Getter.Execute());
	}
}

TPair<float, float> UUserSettingSlider::GetValueRange() const
{
	return TPair<float, float>(MinValue, MaxValue);
}

void UUserSettingSlider::SetValue(float NewValue)
{
	if (Value != NewValue)
	{
		bool bValueInRange = (NewValue >= MinValue) && (NewValue <= MaxValue);
		if (bValueInRange)
		{
			Value = NewValue;
			OnSettingValueChangedEvent.Broadcast();
		}
	}
}

float UUserSettingSlider::GetValue() const
{
	return Value;
}

void UUserSettingSlider::SetSnapSize(float NewStepSize)
{
	SnapSize = NewStepSize;
}

float UUserSettingSlider::GetSnapSize() const
{
	return SnapSize;
}
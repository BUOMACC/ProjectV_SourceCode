// Fill out your copyright notice in the Description page of Project Settings.


#include "UserSettingSelector.h"

UUserSettingSelector::UUserSettingSelector()
{
	Value = 0;
	OptionTexts.Empty();
}

void UUserSettingSelector::Initialize()
{
	Super::Initialize();
}

void UUserSettingSelector::ConfirmValue()
{
	Setter.ExecuteIfBound(Value);
}

void UUserSettingSelector::BindFunction(FSelectorSettingSetterDelegate InSetter, FSelectorSettingGetterDelegate InGetter)
{
	Setter = InSetter;
	Getter = InGetter;

	if (Getter.IsBound())
	{
		SetValue(Getter.Execute());
	}
}

void UUserSettingSelector::SetValue(int32 NewValue)
{
	if (Value != NewValue)
	{
		if (NewValue < 0)
		{
			NewValue += GetOptionCount();
		}
		NewValue %= GetOptionCount();

		Value = NewValue;
		OnSettingValueChangedEvent.Broadcast();
	}
}

int32 UUserSettingSelector::GetValue() const
{
	return Value;
}

void UUserSettingSelector::SetOption(const TArray<FText>& NewOptions)
{
	OptionTexts = NewOptions;
}

int32 UUserSettingSelector::GetOptionCount() const
{
	return OptionTexts.Num();
}
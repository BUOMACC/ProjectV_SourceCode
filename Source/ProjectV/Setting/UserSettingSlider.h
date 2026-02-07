// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Setting/UserSettingBase.h"
#include "UserSettingSlider.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FSliderSettingSetterDelegate, float, NewValue);
DECLARE_DYNAMIC_DELEGATE_RetVal(float, FSliderSettingGetterDelegate);

/**
 *	Setting Entry Slider Widget에 대응되는 객체입니다.
 */
UCLASS()
class PROJECTV_API UUserSettingSlider : public UUserSettingBase
{
	GENERATED_BODY()
	
public:
	UUserSettingSlider();

public:
	virtual void Initialize() override;
	virtual void ConfirmValue() override;

public:
	void SetValueRange(float NewMinValue, float NewMaxValue);
	void BindFunction(FSliderSettingSetterDelegate InSetter, FSliderSettingGetterDelegate InGetter);

	TPair<float, float> GetValueRange() const;

	void SetValue(float NewValue);
	float GetValue() const;

	void SetSnapSize(float NewSnapSize);
	float GetSnapSize() const;

protected:
	float Value;

	float MinValue;
	float MaxValue;

	float SnapSize;

	FSliderSettingSetterDelegate Setter;
	FSliderSettingGetterDelegate Getter;
};

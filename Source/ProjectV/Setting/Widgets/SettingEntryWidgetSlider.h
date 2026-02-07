// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Setting/Widgets/SettingEntryWidgetBase.h"
#include "SettingEntryWidgetSlider.generated.h"

class UTextBlock;
class UEditableText;
class USlider;

UCLASS()
class PROJECTV_API USettingEntryWidgetSlider : public USettingEntryWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	virtual void HandleSettingChanged() override;

	virtual void ConfirmSettingValue() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Setting Entry Widget")
	void InitWidget(FText InSettingName, float InMinValue, float InMaxValue, float InSnapSize);

	UFUNCTION(BlueprintCallable, Category = "Setting Entry Widget")
	void RefreshWidget();

	UFUNCTION(BlueprintCallable, Category = "Setting Entry Widget")
	void SetValue(float NewValue);

protected:
	UFUNCTION(BlueprintPure, Category = "Setting Entry Widget")
	bool CanValueChange(float Value) const;

protected:
	UFUNCTION()
	void HandleSliderValueChanged(float Value);

	UFUNCTION()
	void HandleEditableTextChanged(const FText& Text);

	UFUNCTION()
	void HandleEditableTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Setting Entry Widget")
	float MinValue;

	UPROPERTY(BlueprintReadWrite, Category = "Setting Entry Widget")
	float MaxValue;

	UPROPERTY(BlueprintReadWrite, Category = "Setting Entry Widget")
	float CurrentValue;

	UPROPERTY(BlueprintReadWrite, Category = "Setting Entry Widget")
	float SnapSize;

	UPROPERTY(BlueprintReadWrite, Category = "Setting Entry Widget")
	FText SettingName;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Setting Entry Widget", meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_SettingName;

	UPROPERTY(BlueprintReadOnly, Category = "Setting Entry Widget", meta = (BindWidget))
	TObjectPtr<UEditableText> ETB_Value;

	UPROPERTY(BlueprintReadOnly, Category = "Setting Entry Widget", meta = (BindWidget))
	TObjectPtr<USlider> Slider_Value;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Setting/Widgets/SettingEntryWidgetBase.h"
#include "SettingEntryWidgetSelector.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class PROJECTV_API USettingEntryWidgetSelector : public USettingEntryWidgetBase
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

	virtual void HandleSettingChanged() override;

	virtual void ConfirmSettingValue() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Setting Entry Widget")
	void InitWidget(FText InSettingName, const TArray<FText>& InOptions);

	UFUNCTION(BlueprintCallable, Category = "Setting Entry Widget")
	void RefreshWidget();

	UFUNCTION(BlueprintCallable, Category = "Setting Entry Widget")
	void AddOption(FText InOptionText, bool bWithRefreshWidget = true);

	UFUNCTION(BlueprintCallable, Category = "Setting Entry Widget")
	void SelectOption(int32 NewIndex);

	UFUNCTION(BlueprintCallable, Category = "Setting Entry Widget")
	void SelectOptionNext();

	UFUNCTION(BlueprintCallable, Category = "Setting Entry Widget")
	void SelectOptionPrev();

protected:
	UFUNCTION(BlueprintPure, Category = "Setting Entry Widget")
	bool IsValidOptionIndex(int32 IndexToSelect) const;

	UFUNCTION(BlueprintPure, Category = "Setting Entry Widget")
	int32 GetOptionCount() const;

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Setting Entry Widget")
	int32 SelectedIndex;

	UPROPERTY(BlueprintReadWrite, Category = "Setting Entry Widget")
	TArray<FText> OptionTexts;

	UPROPERTY(BlueprintReadWrite, Category = "Setting Entry Widget")
	FText SettingName;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Setting Entry Widget", meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_SettingName;

	UPROPERTY(BlueprintReadOnly, Category = "Setting Entry Widget", meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_OptionValue;

	UPROPERTY(BlueprintReadOnly, Category = "Setting Entry Widget", meta = (BindWidget))
	TObjectPtr<UButton> Button_OptionNext;

	UPROPERTY(BlueprintReadOnly, Category = "Setting Entry Widget", meta = (BindWidget))
	TObjectPtr<UButton> Button_OptionPrev;

};

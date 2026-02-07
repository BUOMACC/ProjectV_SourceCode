// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Setting/UserSettingBase.h"
#include "UserSettingSelector.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FSelectorSettingSetterDelegate, int32, NewValue);
DECLARE_DYNAMIC_DELEGATE_RetVal(int32, FSelectorSettingGetterDelegate);

/**
 * Setting Entry Selector Widget에 대응되는 객체입니다.
 */
UCLASS()
class PROJECTV_API UUserSettingSelector : public UUserSettingBase
{
	GENERATED_BODY()
	
public:
	UUserSettingSelector();

public:
	virtual void Initialize() override;
	virtual void ConfirmValue() override;

public:
	void BindFunction(FSelectorSettingSetterDelegate InSetter, FSelectorSettingGetterDelegate InGetter);

	void SetValue(int32 NewValue);
	int32 GetValue() const;

	void SetOption(const TArray<FText>& NewOptions);
	const TArray<FText>& GetOptions() const { return OptionTexts; }
	int32 GetOptionCount() const;

protected:
	int32 Value;

	TArray<FText> OptionTexts;

	FSelectorSettingSetterDelegate Setter;
	FSelectorSettingGetterDelegate Getter;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CombatActivatableWidget.h"
#include "Setting/UserSettingSlider.h"
#include "Setting/UserSettingSelector.h"
#include "SettingWidget.generated.h"

class USettingListViewWidget;

#define REGIST_SLIDER(SettingInst, DisplayName, RangeMin, RangeMax, SnapSize, SetterFunc, GetterFunc)							\
		{																														\
			FSliderSettingSetterDelegate SetterDelegate;																		\
			SetterDelegate.BindUFunction(SettingInst, SetterFunc);																\
			FSliderSettingGetterDelegate GetterDelegate;																		\
			GetterDelegate.BindUFunction(SettingInst, GetterFunc);																\
			RegistSettingSlider(FText::FromString(DisplayName), RangeMin, RangeMax, SnapSize, SetterDelegate, GetterDelegate);	\
		}																														\

#define REGIST_SELECTOR(SettingInst, DisplayName, Options, SetterFunc, GetterFunc)												\
		{																														\
			FSelectorSettingSetterDelegate SetterDelegate;																		\
			SetterDelegate.BindUFunction(SettingInst, SetterFunc);																\
			FSelectorSettingGetterDelegate GetterDelegate;																		\
			GetterDelegate.BindUFunction(SettingInst, GetterFunc);																\
			RegistSettingSelector(FText::FromString(DisplayName), Options, SetterDelegate, GetterDelegate);						\
		}																														\


/**
 *  설정 탭 컨텐츠를 표시하는 위젯입니다.
 */
UCLASS()
class PROJECTV_API USettingWidget : public UCombatActivatableWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnActivated() override;

protected:
	UFUNCTION(BlueprintCallable, Category = "Setting Widget")
	void RegistSettingSlider(FText DisplayName, float MinValue, float MaxValue, float SnapSize, FSliderSettingSetterDelegate InSetter, FSliderSettingGetterDelegate InGetter);

	UFUNCTION(BlueprintCallable, Category = "Setting Widget")
	void RegistSettingSelector(FText DisplayName, const TArray<FText>& Options, FSelectorSettingSetterDelegate InSetter, FSelectorSettingGetterDelegate InGetter);

	UFUNCTION(BlueprintCallable, Category = "Setting Widget")
	void ApplySettings();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Setting Widget", meta = (BindWidget))
	TObjectPtr<USettingListViewWidget> SettingListView;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingWidget.h"
#include "SettingListViewWidget.h"
#include "SettingEntryWidgetBase.h"
#include "Setting/UserSettingSlider.h"
#include "Setting/ProjectVUserSettingsShared.h"
#include "Setting/ProjectVUserSettings.h"
#include "Player/CombatPlayerController.h"
#include "Kismet/GameplayStatics.h"

void USettingWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void USettingWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	SettingListView->ClearListItems();

	// Get Shared Setting Instance
	UProjectVUserSettingsShared* SharedSettings = nullptr;
	if (ACombatPlayerController* CombatPC = Cast<ACombatPlayerController>(GetOwningPlayer()))
	{
		SharedSettings = CombatPC->GetSharedSettings();
	}

	// Get Local Setting Instance
	UProjectVUserSettings* LocalSettings = UProjectVUserSettings::GetProjectVUserSettings();

	TArray<FText> FourOption = { FText::FromString(TEXT("낮음")), FText::FromString(TEXT("중간")), FText::FromString(TEXT("높음")), FText::FromString(TEXT("매우 높음")) };
	TArray<FText> ToggleOption = { FText::FromString(TEXT("끄기")), FText::FromString(TEXT("켜기")) };

	// Regist Shared Settings
	REGIST_SLIDER(SharedSettings, TEXT("마우스 감도"), 0.f, 10.f, 0.02f, TEXT("SetMouseSensitivity"), TEXT("GetMouseSensitivity"));
	REGIST_SELECTOR(SharedSettings, TEXT("부드러운 카메라"), ToggleOption, TEXT("SetSmoothCamera"), TEXT("GetSmoothCamera"));
	
	// Regist Local Setings
	REGIST_SELECTOR(LocalSettings, TEXT("모션블러"), ToggleOption, TEXT("SetMotionBlur"), TEXT("GetMotionBlur"));

	REGIST_SLIDER(LocalSettings, TEXT("밝기"), 1.7f, 2.7f, 0.02f, TEXT("SetDisplayGamma"), TEXT("GetDisplayGamma"));
	REGIST_SLIDER(LocalSettings, TEXT("최대 프레임"), 30.f, 300.f, 1.f, TEXT("SetFrameRate"), TEXT("GetFrameRate"));
	REGIST_SLIDER(LocalSettings, TEXT("전체 음량"), 0.f, 2.f, 0.1f, TEXT("SetMasterVolume"), TEXT("GetMasterVolume"));
	REGIST_SLIDER(LocalSettings, TEXT("음악 음량"), 0.f, 2.f, 0.1f, TEXT("SetMusicVolume"), TEXT("GetMusicVolume"));
	REGIST_SLIDER(LocalSettings, TEXT("효과 음량"), 0.f, 2.f, 0.1f, TEXT("SetSFXVolume"), TEXT("GetSFXVolume"));

	REGIST_SELECTOR(LocalSettings, TEXT("글로벌 일루미네이션"), FourOption, TEXT("SetGlobalIlluminationQuality"), TEXT("GetGlobalIlluminationQuality"));
	REGIST_SELECTOR(LocalSettings, TEXT("그림자"), FourOption, TEXT("SetShadowQuality"), TEXT("GetShadowQuality"));
	REGIST_SELECTOR(LocalSettings, TEXT("반사 품질"), FourOption, TEXT("SetReflectionQuality"), TEXT("GetReflectionQuality"));
	REGIST_SELECTOR(LocalSettings, TEXT("후처리 품질"), FourOption, TEXT("SetPostProcessingQuality"), TEXT("GetPostProcessingQuality"));
}

void USettingWidget::RegistSettingSlider(FText DisplayName, float MinValue, float MaxValue, float SnapSize, FSliderSettingSetterDelegate InSetter, FSliderSettingGetterDelegate InGetter)
{
	UUserSettingSlider* SliderSetting = NewObject<UUserSettingSlider>();
	SliderSetting->Initialize();
	SliderSetting->SetDisplayName(DisplayName);
	SliderSetting->SetValueRange(MinValue, MaxValue);
	SliderSetting->SetSnapSize(SnapSize);
	SliderSetting->BindFunction(InSetter, InGetter);

	SettingListView->AddItem(SliderSetting);
}

void USettingWidget::RegistSettingSelector(FText DisplayName, const TArray<FText>& Options, FSelectorSettingSetterDelegate InSetter, FSelectorSettingGetterDelegate InGetter)
{
	UUserSettingSelector* SelectorSetting = NewObject<UUserSettingSelector>();
	SelectorSetting->Initialize();
	SelectorSetting->SetDisplayName(DisplayName);
	SelectorSetting->SetOption(Options);
	SelectorSetting->BindFunction(InSetter, InGetter);

	SettingListView->AddItem(SelectorSetting);
}

void USettingWidget::ApplySettings()
{
	for (UObject* Item : SettingListView->GetListItems())
	{
		if (USettingEntryWidgetBase* Entry = SettingListView->GetEntryWidgetFromItem<USettingEntryWidgetBase>(Item))
		{
			Entry->ConfirmSettingValue();
		}
	}

	// Save Shared Settings
	UProjectVUserSettingsShared* SharedSettings = nullptr;
	if (ACombatPlayerController* CombatPC = Cast<ACombatPlayerController>(GetOwningPlayer()))
	{
		SharedSettings = CombatPC->GetSharedSettings();
		SharedSettings->ApplySetting();
		SharedSettings->SaveSetting();
	}

	// Save Local Settings
	UProjectVUserSettings* LocalSettings = UProjectVUserSettings::GetProjectVUserSettings();
	LocalSettings->ApplySettings(false);

}
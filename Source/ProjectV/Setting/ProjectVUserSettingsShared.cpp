// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectVUserSettingsShared.h"
#include "Kismet/GameplayStatics.h"

UProjectVUserSettingsShared::UProjectVUserSettingsShared()
{
	MouseSensitivity = 0.5f;
	SmoothCamera = false;
}

UProjectVUserSettingsShared* UProjectVUserSettingsShared::LoadOrCreateSettings()
{
	UProjectVUserSettingsShared* Result = nullptr;

	int32 UserIndex = 0;
	if (UGameplayStatics::DoesSaveGameExist(SETTING_SLOT_NAME, UserIndex))
	{
		USaveGame* Setting = UGameplayStatics::LoadGameFromSlot(SETTING_SLOT_NAME, UserIndex);
		Result = Cast<UProjectVUserSettingsShared>(Setting);
	}

	if (Result == nullptr)
	{
		Result = Cast<UProjectVUserSettingsShared>(UGameplayStatics::CreateSaveGameObject(UProjectVUserSettingsShared::StaticClass()));
	}

	Result->Initialize();
	Result->ApplySetting();

	return Result;
}

void UProjectVUserSettingsShared::ApplySetting()
{
	OnSettingApplied.Broadcast(this);
}

void UProjectVUserSettingsShared::SaveSetting()
{
	int32 UserIndex = 0;
	UGameplayStatics::SaveGameToSlot(this, SETTING_SLOT_NAME, UserIndex);
}

void UProjectVUserSettingsShared::Initialize()
{
	// ...
}

void UProjectVUserSettingsShared::SetMouseSensitivity(float InNewSensitivity)
{
	MouseSensitivity = InNewSensitivity;
}

float UProjectVUserSettingsShared::GetMouseSensitivity() const
{
	return MouseSensitivity;
}

void UProjectVUserSettingsShared::SetSmoothCamera(int32 bNewSmoothCameraFlag)
{
	if (SmoothCamera != bNewSmoothCameraFlag)
	{
		SmoothCamera = bNewSmoothCameraFlag;
	}
}

int32 UProjectVUserSettingsShared::GetSmoothCamera() const
{
	return SmoothCamera;
}

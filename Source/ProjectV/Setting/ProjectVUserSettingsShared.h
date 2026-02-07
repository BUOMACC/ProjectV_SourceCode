// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ProjectVUserSettingsShared.generated.h"

static FString SETTING_SLOT_NAME = TEXT("SharedUserSettings");

DECLARE_EVENT_OneParam(UProjectVUserSettingsShared, FOnSettingAppliedEvent, UProjectVUserSettingsShared* Settings);

/**
 *	SaveGame으로 저장되고 공유되는 사용자 설정입니다.
 *	 ex) 마우스 감도, 키보드 바인딩...
 */
UCLASS()
class PROJECTV_API UProjectVUserSettingsShared : public USaveGame
{
	GENERATED_BODY()

public:
	UProjectVUserSettingsShared();

	static UProjectVUserSettingsShared* LoadOrCreateSettings();

	void ApplySetting();
	void SaveSetting();

public:
	FOnSettingAppliedEvent OnSettingApplied;

protected:
	virtual void Initialize();

public:
	UFUNCTION(BlueprintCallable, Category = "Shared Settings")
	void SetMouseSensitivity(float InNewSensitivity);

	UFUNCTION(BlueprintPure, Category = "Shared Setting")
	float GetMouseSensitivity() const;

	UFUNCTION(BlueprintCallable, Category = "Shared Settings")
	void SetSmoothCamera(int32 bNewSmoothCameraFlag);

	UFUNCTION(BlueprintPure, Category = "Shared Setting")
	int32 GetSmoothCamera() const;

private:
	UPROPERTY()
	float MouseSensitivity;

	UPROPERTY()
	int32 SmoothCamera;
};

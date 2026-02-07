// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "ProjectVUserSettings.generated.h"

class USoundMix;
class USoundClass;

/**
 *	Config에 저장되고 로컬에서만 사용되는 사용자 설정입니다.
 *	 ex) 그래픽 설정, 수직 동기화, 해상도...
 */
UCLASS()
class PROJECTV_API UProjectVUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	UProjectVUserSettings();

	UFUNCTION(BlueprintCallable, Category = Settings)
	static UProjectVUserSettings* GetProjectVUserSettings();

public:
	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetDisplayGamma(float InNewGamma);

	UFUNCTION(BlueprintPure, Category = "Settings")
	float GetDisplayGamma() const;

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetFrameRate(float InNewFrameRate);

	UFUNCTION(BlueprintPure, Category = "Settings")
	float GetFrameRate() const;

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetMasterVolume(float InNewVolume);

	UFUNCTION(BlueprintPure, Category = "Settings")
	float GetMasterVolume() const;

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetMusicVolume(float InNewVolume);

	UFUNCTION(BlueprintPure, Category = "Settings")
	float GetMusicVolume() const;

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetSFXVolume(float InNewVolume);

	UFUNCTION(BlueprintPure, Category = "Settings")
	float GetSFXVolume() const;

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetMotionBlur(int32 InNewFlag);

	UFUNCTION(BlueprintPure, Category = "Settings")
	int32 GetMotionBlur() const;
	
private:
	UPROPERTY(Config)
	float DisplayGamma;

	UPROPERTY(Config)
	float FrameRate;

	UPROPERTY(Config)
	float MasterVolume;

	UPROPERTY(Config)
	float MusicVolume;

	UPROPERTY(Config)
	float SFXVolume;

	UPROPERTY(Config)
	int32 MotionBlur;

private:
	UPROPERTY(Transient)
	TObjectPtr<USoundMix> SoundMix;

	UPROPERTY(Transient)
	TObjectPtr<USoundClass> MasterSound;

	UPROPERTY(Transient)
	TObjectPtr<USoundClass> MusicSound;

	UPROPERTY(Transient)
	TObjectPtr<USoundClass> SFXSound;
};

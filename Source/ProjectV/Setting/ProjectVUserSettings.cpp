// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectVUserSettings.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

UProjectVUserSettings::UProjectVUserSettings()
{
	DisplayGamma = 2.2f;
	FrameRate = 120.f;

	MasterVolume = 1.f;
	MusicVolume = 1.f;
	SFXVolume = 1.f;

	MotionBlur = 1;

	static ConstructorHelpers::FObjectFinder<USoundMix> SoundMixFinder(TEXT("/Game/Asset/SFX/MasterSoundMix.MasterSoundMix"));
	if (SoundMixFinder.Succeeded())
	{
		SoundMix = SoundMixFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundClass> MasterSoundFinder(TEXT("/Game/Asset/SFX/MasterSound.MasterSound"));
	if (MasterSoundFinder.Succeeded())
	{
		MasterSound = MasterSoundFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundClass> SFXSoundFinder(TEXT("/Game/Asset/SFX/SFXSound.SFXSound"));
	if (SFXSoundFinder.Succeeded())
	{
		SFXSound = SFXSoundFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundClass> MusicSoundFinder(TEXT("/Game/Asset/SFX/BGMSound.BGMSound"));
	if (MusicSoundFinder.Succeeded())
	{
		MusicSound = MusicSoundFinder.Object;
	}
}

UProjectVUserSettings* UProjectVUserSettings::GetProjectVUserSettings()
{
	return Cast<UProjectVUserSettings>(GEngine->GetGameUserSettings());
}

void UProjectVUserSettings::SetDisplayGamma(float InNewGamma)
{
	DisplayGamma = InNewGamma;
	if (GEngine)
	{
		GEngine->DisplayGamma = DisplayGamma;
	}
}

float UProjectVUserSettings::GetDisplayGamma() const
{
	return DisplayGamma;
}

void UProjectVUserSettings::SetFrameRate(float InNewFrameRate)
{
	FrameRate = InNewFrameRate;
	SetFrameRateLimit(FrameRate);
}

float UProjectVUserSettings::GetFrameRate() const
{
	return FrameRate;
}

void UProjectVUserSettings::SetMasterVolume(float InNewVolume)
{
	MasterVolume = InNewVolume;
	UGameplayStatics::SetSoundMixClassOverride(GEngine->GetCurrentPlayWorld(), SoundMix, MasterSound, MasterVolume);
}

float UProjectVUserSettings::GetMasterVolume() const
{
	return MasterVolume;
}

void UProjectVUserSettings::SetMusicVolume(float InNewVolume)
{
	MusicVolume = InNewVolume;
	UGameplayStatics::SetSoundMixClassOverride(GEngine->GetCurrentPlayWorld(), SoundMix, MusicSound, MusicVolume);
}

float UProjectVUserSettings::GetMusicVolume() const
{
	return MusicVolume;
}

void UProjectVUserSettings::SetSFXVolume(float InNewVolume)
{
	SFXVolume = InNewVolume;
	UGameplayStatics::SetSoundMixClassOverride(GEngine->GetCurrentPlayWorld(), SoundMix, SFXSound, SFXVolume);
}

float UProjectVUserSettings::GetSFXVolume() const
{
	return SFXVolume;
}

void UProjectVUserSettings::SetMotionBlur(int32 InNewFlag)
{
	MotionBlur = InNewFlag;

	FString Command = FString::Printf(TEXT("r.MotionBlurQuality %d"), MotionBlur);
	UKismetSystemLibrary::ExecuteConsoleCommand(GEngine->GetCurrentPlayWorld(), Command);
}

int32 UProjectVUserSettings::GetMotionBlur() const
{
	return MotionBlur;
}
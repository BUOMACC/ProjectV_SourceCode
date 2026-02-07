// Fill out your copyright notice in the Description page of Project Settings.


#include "FootstepData.h"

USoundBase* UFootstepData::FindFootstepSound(TEnumAsByte<EPhysicalSurface> InSurfaceType)
{
	if (auto SoundPtr = FootstepSoundMap.Find(InSurfaceType))
	{
		return *SoundPtr;
	}
	return nullptr;
}

UNiagaraSystem* UFootstepData::FindFootstepEffect(TEnumAsByte<EPhysicalSurface> InSurfaceType)
{
	if (auto SoundPtr = FootstepEffectMap.Find(InSurfaceType))
	{
		return *SoundPtr;
	}
	return nullptr;
}
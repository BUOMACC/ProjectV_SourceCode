// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FootstepData.generated.h"

class USoundBase;
class UNiagaraSystem;

UCLASS()
class PROJECTV_API UFootstepData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	USoundBase* FindFootstepSound(TEnumAsByte<EPhysicalSurface> InSurfaceType);
	UNiagaraSystem* FindFootstepEffect(TEnumAsByte<EPhysicalSurface> InSurfaceType);

public:
	/* 애님 노티파이로부터 Footstep 효과음을 재생하라고 호출받았을 때 대응되는 사운드 에셋입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Footstep Data")
	TMap<TEnumAsByte<EPhysicalSurface>, TObjectPtr<USoundBase>> FootstepSoundMap;

	/* 애님 노티파이로부터 Footstep을 재생하라고 호출받았을 때 대응되는 에셋입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Footstep Data")
	TMap<TEnumAsByte<EPhysicalSurface>, TObjectPtr<UNiagaraSystem>> FootstepEffectMap;
};

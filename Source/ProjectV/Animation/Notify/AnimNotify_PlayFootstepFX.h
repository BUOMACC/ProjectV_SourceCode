// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_PlayFootstepFX.generated.h"

/**
 *	현재 밟고있는 땅의 재질에 따라 다른 사운드를 재생하는 노티파이
 */
UCLASS(meta = (DisplayName = "Play Footstep FX"))
class PROJECTV_API UAnimNotify_PlayFootstepFX : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UAnimNotify_PlayFootstepFX();

private:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere, Category = "Sound")
	float VolumeMultiplier;

	UPROPERTY(EditAnywhere, Category = "Sound")
	float PitchMultiplier;

	UPROPERTY(EditAnywhere, Category = "Effect")
	FName BoneName;

	UPROPERTY(EditAnywhere, Category = "General")
	TEnumAsByte<ETraceTypeQuery> GroundTraceChannel;

	UPROPERTY(EditAnywhere, Category = "General")
	bool bIgnoreVFX;
};

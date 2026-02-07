// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_MakeNoise.generated.h"

/**
 *	AI Perception : Hearing 관련 Noise 발생 노티파이
 */
UCLASS(meta = (DisplayName = "Make Noise"))
class PROJECTV_API UAnimNotify_MakeNoise : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UAnimNotify_MakeNoise();

private:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Noise Settings")
	float Loudness;

	UPROPERTY(EditAnywhere, Category = "Noise Settings")
	float Range;
};

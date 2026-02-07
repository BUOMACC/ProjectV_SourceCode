// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_AllowInputBuffer.generated.h"

/**
 *	일정 시간동안 InputBuffer에 입력을 저장
 */
UCLASS(meta = (DisplayName = "AllowInputBuffer"))
class PROJECTV_API UAnimNotifyState_AllowInputBuffer : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UAnimNotifyState_AllowInputBuffer();

private:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};

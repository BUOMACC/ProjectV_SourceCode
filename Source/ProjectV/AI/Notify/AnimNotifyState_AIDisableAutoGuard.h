// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AnimNotifyState_AIBase.h"
#include "AnimNotifyState_AIDisableAutoGuard.generated.h"

/**
 *	AI 전용 - 오토가드 비활성화
 *   - BeginAttack() 이벤트 등으로 피격 전 가드를 자동으로 올리는 행위를 비활성합니다.
 *	   노티파이가 종료되면 다시 자동으로 가드를 올릴 수 있습니다.
 */
UCLASS(meta = (DisplayName = "AI Disable AutoGuard"))
class PROJECTV_API UAnimNotifyState_AIDisableAutoGuard : public UAnimNotifyState_AIBase
{
	GENERATED_BODY()
	
public:
	UAnimNotifyState_AIDisableAutoGuard();

private:
	virtual FString GetNotifyName_Implementation() const override;

protected:
	virtual void NotifyBeginAI(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEndAI(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};

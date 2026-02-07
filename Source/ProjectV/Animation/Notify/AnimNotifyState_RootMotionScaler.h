// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_RootMotionScaler.generated.h"

class UCharacterMovementComponent;

/**
 *	캐릭터의 정면 방향으로 충돌이 감지되면, RootMotion의 이동 스케일을 설정함
 *	과도하게 RootMotion이 이동하는 현상을 방지
 *	*** DEPRECATED - MotionWarping과 동시에 사용불가능 ***
 */
UCLASS(meta = (DisplayName = "RM Scaler"))
class PROJECTV_API UAnimNotifyState_RootMotionScaler : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UAnimNotifyState_RootMotionScaler();

private:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	void UnbindRootMotionScalerFunction(USkeletalMeshComponent* MeshComp);
	bool DoCheck(USkeletalMeshComponent* MeshComp) const;
	FTransform ConvertRootMotionDeltaLocation(const FTransform& InDeltaTransform, UCharacterMovementComponent* CharacterMovementComponent, float DeltaSeconds);

public:
	/* RootMotion Scale에 곱해질 값입니다. (1.0 = 100%) */
	UPROPERTY(EditAnywhere, Category = "RootMotion", meta = (AllowPrivateAccess = "true"))
	float RootMotionScale;

	/* 충돌체크를 포함할까요? (활성화시 충돌이 감지되어야만 기능이 동작합니다.) */
	UPROPERTY(EditAnywhere, Category = "RootMotion", meta = (AllowPrivateAccess = "true"))
	bool bNeedCheckCollision;

	/* 앞으로 체크할 거리입니다. */
	UPROPERTY(EditAnywhere, Category = "RootMotion", meta = (AllowPrivateAccess = "true", EditCondition = "bNeedCheckCollision == true", EditConditionHides))
	float ForwardDistance;

	/* 효과를 적용하기 위해 충돌을 체크해야하는 채널입니다. */
	UPROPERTY(EditAnywhere, Category = "RootMotion", meta = (AllowPrivateAccess = "true", EditCondition = "bNeedCheckCollision == true", EditConditionHides))
	TEnumAsByte<ETraceTypeQuery> TraceChannel;

	UPROPERTY(EditAnywhere, Category = "RootMotion", meta = (AllowPrivateAccess = "true", EditCondition = "bNeedCheckCollision == true", EditConditionHides))
	bool bDrawDebug;
};

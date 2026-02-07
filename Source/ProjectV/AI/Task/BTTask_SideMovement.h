// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SideMovement.generated.h"

/**
 *	대치 상황에서 좌또는 우측으로 이동하는 Task
 *	이동하는동안 캐릭터가 이동 방향으로 회전하지 않습니다.
 */
UCLASS()
class PROJECTV_API UBTTask_SideMovement : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_SideMovement();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	void TurnSideMovementMode(AActor* Target, bool bEnableMode);

private:
	/* 태스크의 실행 시간(초)입니다. (최소 ~ 최대, 특정 상황에 의해 조기종료될 수 있습니다.) */
	UPROPERTY(EditAnywhere, Category = "Side Movement", meta = (AllowPrivateAccess = "true"))
	FVector2D ExecuteTimeMinMax;

	/* true면 왼쪽으로 이동합니다. */
	UPROPERTY(EditAnywhere, Category = "Side Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsLeftMovement;

	/* 입력받은 Blackboard Key값의 대상과의 최소 거리입니다. (해당범위로 들어오면 태스크가 종료됩니다.) */
	UPROPERTY(EditAnywhere, Category = "Side Movement", meta = (AllowPrivateAccess = "true"))
	float StopDistance;

private:
	float CurrentTime;
	float TargetTime;

	float TraceCheckDistance;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_RotateTo.generated.h"

/**
 *	선택한 대상을 기준으로 Yaw 회전하는 태스크
 *	 - Vector를 선택한 경우 -> Vector를 바라보는 방향
 *	 - Object(Actor)를 선택한 경우 -> Actor를 향하는 방향
 *	 - Rotator를 선택한 경우 -> Rotator의 방향
 */
UCLASS()
class PROJECTV_API UBTTask_RotateTo : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_RotateTo();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	/* 해당 시간동안 목표에 도달하도록 회전합니다. */
	UPROPERTY(EditAnywhere, Category = "Rotate To")
	float RotateTime;

private:
	FRotator StartRotation;
	FRotator TargetRotation;

	float CurrentTime;
};

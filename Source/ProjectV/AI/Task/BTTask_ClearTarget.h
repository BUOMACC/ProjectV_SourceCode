// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ClearTarget.generated.h"

/**
 *	현재 타겟을 초기화하는 태스크
 */
UCLASS()
class PROJECTV_API UBTTask_ClearTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_ClearTarget();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};

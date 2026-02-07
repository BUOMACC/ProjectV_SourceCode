// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Success.generated.h"

/*
 *	항상 성공처리되는 Task
 *	Selector나 Sequence 등에서 Dummy Task로 사용함
 */
UCLASS()
class PROJECTV_API UBTTask_Success : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_Success();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

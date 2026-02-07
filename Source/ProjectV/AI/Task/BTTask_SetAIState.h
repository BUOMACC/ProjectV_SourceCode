// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ProjectVCommonEnums.h"
#include "BTTask_SetAIState.generated.h"

/**
 *	AI의 상태를 변경하는 태스크
 */
UCLASS()
class PROJECTV_API UBTTask_SetAIState : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_SetAIState();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	EAIState NewState;
};

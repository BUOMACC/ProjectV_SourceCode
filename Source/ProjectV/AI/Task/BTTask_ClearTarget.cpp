// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ClearTarget.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ClearTarget::UBTTask_ClearTarget()
{
	NodeName = TEXT("Clear Target");
}

EBTNodeResult::Type UBTTask_ClearTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent())
	{
		Blackboard->ClearValue(TEXT("TargetActor"));
	}
	return EBTNodeResult::Succeeded;
}

#if WITH_EDITOR
void UBTTask_ClearTarget::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ResetSenseValue.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ResetSenseValue::UBTTask_ResetSenseValue()
{
	NodeName = TEXT("Reset Sense Value");
}

EBTNodeResult::Type UBTTask_ResetSenseValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent())
	{
		Blackboard->SetValueAsFloat(TEXT("SenseValue"), 0.f);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

void UBTTask_ResetSenseValue::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

#if WITH_EDITOR
void UBTTask_ResetSenseValue::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

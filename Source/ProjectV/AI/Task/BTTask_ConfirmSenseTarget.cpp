// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ConfirmSenseTarget.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ConfirmSenseTarget::UBTTask_ConfirmSenseTarget()
{
	NodeName = TEXT("Confirm Target");
}

EBTNodeResult::Type UBTTask_ConfirmSenseTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent())
	{
		UObject* TargetObject = Blackboard->GetValueAsObject(TEXT("TargetActor"));
		UObject* SenseObject = Blackboard->GetValueAsObject(TEXT("LastSenseActor"));
		Blackboard->SetValueAsObject(TEXT("TargetActor"), SenseObject);
		Blackboard->ClearValue(TEXT("LastSenseActor"));
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

void UBTTask_ConfirmSenseTarget::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

#if WITH_EDITOR
void UBTTask_ConfirmSenseTarget::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

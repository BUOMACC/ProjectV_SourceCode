// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetAIState.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetAIState::UBTTask_SetAIState()
{
	NodeName = TEXT("Set AI State");

	NewState = EAIState::Peace;
}

EBTNodeResult::Type UBTTask_SetAIState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent())
	{
		Blackboard->SetValueAsEnum(TEXT("State"), static_cast<uint8>(NewState));
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

#if WITH_EDITOR
void UBTTask_SetAIState::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

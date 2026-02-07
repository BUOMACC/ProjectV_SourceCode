// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPoint.h"
#include "AIController.h"
#include "AI/PatrolPathActor.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/EnemyCharacter.h"

UBTTask_FindPatrolPoint::UBTTask_FindPatrolPoint()
{
	NodeName = TEXT("Find Patrol Point");
}

EBTNodeResult::Type UBTTask_FindPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard)
	{
		return EBTNodeResult::Failed;
	}

	if (APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn())
	{
		if (AEnemyCharacter* OwnerCharacter = Cast<AEnemyCharacter>(OwnerPawn))
		{
			APatrolPathActor* PatrolPath = OwnerCharacter->GetPatrolPathActor();
			if (!IsValid(PatrolPath))
			{
				return EBTNodeResult::Failed;
			}

			FVector PatrolPoint = FVector::ZeroVector;
			if (PatrolPath->GetPatrolPoint(OwnerCharacter, PatrolPoint))
			{
				Blackboard->SetValueAsVector(TEXT("PatrolLocation"), PatrolPoint);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}

#if WITH_EDITOR
void UBTTask_FindPatrolPoint::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

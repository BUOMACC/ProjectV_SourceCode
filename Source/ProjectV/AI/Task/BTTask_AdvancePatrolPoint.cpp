// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AdvancePatrolPoint.h"
#include "AIController.h"
#include "AI/PatrolPathActor.h"
#include "Character/EnemyCharacter.h"

UBTTask_AdvancePatrolPoint::UBTTask_AdvancePatrolPoint()
{
	NodeName = TEXT("Advance Patrol Point");
}

EBTNodeResult::Type UBTTask_AdvancePatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn())
	{
		if (AEnemyCharacter* OwnerCharacter = Cast<AEnemyCharacter>(OwnerPawn))
		{
			APatrolPathActor* PatrolPath = OwnerCharacter->GetPatrolPathActor();
			if (!IsValid(PatrolPath))
			{
				return EBTNodeResult::Failed;
			}

			FVector NextPoint = FVector::ZeroVector;
			bool bSuccess = PatrolPath->AdvanceNextPatrolPoint(OwnerCharacter, NextPoint);
			return (bSuccess) ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
		}
	}
	return EBTNodeResult::Failed;
}

#if WITH_EDITOR
void UBTTask_AdvancePatrolPoint::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

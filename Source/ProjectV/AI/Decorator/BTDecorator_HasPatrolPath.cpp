// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_HasPatrolPath.h"
#include "AIController.h"
#include "AI/PatrolPathActor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/EnemyCharacter.h"

UBTDecorator_HasPatrolPath::UBTDecorator_HasPatrolPath()
{
	NodeName = TEXT("[Custom] HasPatrolPath");
}

FString UBTDecorator_HasPatrolPath::GetStaticDescription() const
{
	FString Description = FString::Printf(TEXT("%s: %s"), *Super::GetStaticDescription(), *(TEXT("Self")));
	return Description;
}

bool UBTDecorator_HasPatrolPath::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return false;
	}

	if (APawn* TargetObject = OwnerComp.GetAIOwner()->GetPawn())
	{
		if (AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(TargetObject))
		{
			return IsValid(EnemyCharacter->GetPatrolPathActor());
		}
	}
	return false;
}

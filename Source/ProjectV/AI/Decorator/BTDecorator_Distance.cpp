// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_Distance.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UBTDecorator_Distance::UBTDecorator_Distance()
{
	NodeName = TEXT("[Custom] Distance");
	bNotifyTick = true;
}

FString UBTDecorator_Distance::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s: %s, %f"), *Super::GetStaticDescription(), *Super::GetSelectedBlackboardKey().ToString(), Distance);
}

uint16 UBTDecorator_Distance::GetInstanceMemorySize() const
{
	return sizeof(FBTDistanceCheckDecoratorMemory);
}

bool UBTDecorator_Distance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return ConditionCheck(OwnerComp, NodeMemory);
}

void UBTDecorator_Distance::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTDistanceCheckDecoratorMemory* DecoratorMemory = CastInstanceNodeMemory<FBTDistanceCheckDecoratorMemory>(NodeMemory);
	DecoratorMemory->bLastRawResult = ConditionCheck(OwnerComp, NodeMemory);
}

void UBTDecorator_Distance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FBTDistanceCheckDecoratorMemory* DecoratorMemory = CastInstanceNodeMemory<FBTDistanceCheckDecoratorMemory>(NodeMemory);

	bool bResult = ConditionCheck(OwnerComp, NodeMemory);
	if (IsInversed())
	{
		bResult = !bResult;
	}

	if (bResult != DecoratorMemory->bLastRawResult)
	{
		DecoratorMemory->bLastRawResult = bResult;
		OwnerComp.RequestExecution(this);
	}
}

bool UBTDecorator_Distance::ConditionCheck(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return false;
	}

	if (UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()))
	{
		if (AActor* TargetActor = Cast<AActor>(TargetObject))
		{
			// Check Distance
			const FVector& TargetLocation = TargetActor->GetActorLocation();
			const FVector& OwnerLocation = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();

			float OwnerToTargetDistance = FVector::Distance(TargetLocation, OwnerLocation);
			return (OwnerToTargetDistance <= Distance);
		}
	}

	return false;
}

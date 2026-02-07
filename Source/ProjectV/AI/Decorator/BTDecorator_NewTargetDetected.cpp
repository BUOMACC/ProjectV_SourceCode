// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_NewTargetDetected.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UBTDecorator_NewTargetDetected::UBTDecorator_NewTargetDetected()
{
    NodeName = TEXT("[Custom] New Target Detected");
}

FString UBTDecorator_NewTargetDetected::GetStaticDescription() const
{
    return Super::GetStaticDescription();
}

bool UBTDecorator_NewTargetDetected::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return false;
	}

	UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor"));
	UObject* LastSenseObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("LastSenseActor"));

	bool bIsChanged = (TargetObject != LastSenseObject);
	if (IsValid(LastSenseObject) && bIsChanged)
	{
		return true;
	}

	return false;
}

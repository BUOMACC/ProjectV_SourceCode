// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_ChanceOf.h"

UBTDecorator_ChanceOf::UBTDecorator_ChanceOf()
{
	NodeName = TEXT("[Custom] Chance Of");
}

FString UBTDecorator_ChanceOf::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s: %d%%"), *Super::GetStaticDescription(), Chance);
}

bool UBTDecorator_ChanceOf::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	int32 RandomNumber = FMath::RandRange(1, 100);
	if (RandomNumber <= Chance)
	{
		return true;
	}
	return false;
}
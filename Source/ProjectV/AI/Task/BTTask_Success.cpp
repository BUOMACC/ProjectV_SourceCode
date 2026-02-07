// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Success.h"

UBTTask_Success::UBTTask_Success()
{
	NodeName = TEXT("Success");
}

EBTNodeResult::Type UBTTask_Success::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Succeeded;
}

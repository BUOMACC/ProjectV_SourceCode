// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_WaitForTagRemove.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AIController.h"

UBTTask_WaitForTagRemove::UBTTask_WaitForTagRemove()
{
	NodeName = TEXT("Wait For TagRemove");

	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTTask_WaitForTagRemove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}

void UBTTask_WaitForTagRemove::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerPawn))
		{
			if (ASC->HasMatchingGameplayTag(TagToCheck))
			{
				return;
			}
		}
	}
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

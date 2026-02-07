// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CancelAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AIController.h"

UBTTask_CancelAbility::UBTTask_CancelAbility()
{
	NodeName = TEXT("Cancel Ability");
}

EBTNodeResult::Type UBTTask_CancelAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerPawn))
		{
			ASC->CancelAbilities(&TagContainer, nullptr, nullptr);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_TagCheck.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

UBTDecorator_TagCheck::UBTDecorator_TagCheck()
{
	NodeName = TEXT("[Custom] Tag Check");
	bNotifyTick = true;
}

FString UBTDecorator_TagCheck::GetStaticDescription() const
{
	FString Description = FString::Printf(TEXT("%s: %s"), *Super::GetStaticDescription(), *Super::GetSelectedBlackboardKey().ToString());
	for (const FGameplayTag& Tag : TagContainer)
	{
		Description += TEXT("\n+") + Tag.ToString();
	}
	return Description;
}

bool UBTDecorator_TagCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
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
			// Get ASC, Check Tag
			if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor))
			{
				return ASC->HasAnyMatchingGameplayTags(TagContainer);
			}
		}
	}
	return false;
}

void UBTDecorator_TagCheck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return;
	}

	UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey());
	if (!TargetObject)
	{
		return;
	}

	if (AActor* TargetActor = Cast<AActor>(TargetObject))
	{
		// Get ASC, Check Tag
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor))
		{
			if (ASC->HasAnyMatchingGameplayTags(TagContainer))
			{
				// 태그가 생기면 재평가 요청
				OwnerComp.RequestExecution(this);
			}
		}
	}
}

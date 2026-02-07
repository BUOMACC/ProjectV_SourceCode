// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GameplayTagContainer.h"
#include "BTTask_CancelAbility.generated.h"

/**
 *	어빌리티 취소 Task
 */
UCLASS()
class PROJECTV_API UBTTask_CancelAbility : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_CancelAbility();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Cancel Ability")
	FGameplayTagContainer TagContainer;
};

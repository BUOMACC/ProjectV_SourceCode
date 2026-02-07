// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GameplayTagContainer.h"
#include "BTTask_WaitForTagRemove.generated.h"

/*
 *	매 Tick마다 Tag 체크해서 없으면 Success 처리, 아니면 대기
 */
UCLASS()
class PROJECTV_API UBTTask_WaitForTagRemove : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_WaitForTagRemove();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Wait For Tag Remove")
	FGameplayTag TagToCheck;
};

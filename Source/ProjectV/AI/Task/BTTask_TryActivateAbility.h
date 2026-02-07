// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TryActivateAbility.generated.h"

class UGameplayAbility;

/*
 *	어빌리티 실행 Task
 */
UCLASS()
class PROJECTV_API UBTTask_TryActivateAbility : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_TryActivateAbility();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "Try Activate Ability")
	TSubclassOf<UGameplayAbility> AbilityClass;
};

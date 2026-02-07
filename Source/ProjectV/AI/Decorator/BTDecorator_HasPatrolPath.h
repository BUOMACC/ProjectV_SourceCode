// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_HasPatrolPath.generated.h"

/**
 *	AI 캐릭터가 순찰할 수 있는 경로가 있는지 판단합니다.
 */
UCLASS()
class PROJECTV_API UBTDecorator_HasPatrolPath : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_HasPatrolPath();

private:
	virtual FString GetStaticDescription() const override;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
};

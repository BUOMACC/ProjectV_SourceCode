// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_NewTargetDetected.generated.h"

/**
 *	새로운 타겟이 감지된 경우를 체크합니다.
 *  null이 아닌 대상이 감지되면 true를 반환합니다.
 */
UCLASS()
class PROJECTV_API UBTDecorator_NewTargetDetected : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_NewTargetDetected();

private:
	virtual FString GetStaticDescription() const override;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};

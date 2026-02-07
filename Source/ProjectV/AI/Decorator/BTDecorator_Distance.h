// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_Distance.generated.h"

struct FBTDistanceCheckDecoratorMemory
{
	bool bLastRawResult;
};

/*
 *	대상이 Distance 내에 있다면 true
 */
UCLASS()
class PROJECTV_API UBTDecorator_Distance : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTDecorator_Distance();

private:
	virtual FString GetStaticDescription() const override;

protected:
	virtual uint16 GetInstanceMemorySize() const override;
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	bool ConditionCheck(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;

protected:
	UPROPERTY(EditAnywhere, Category = "Distance")
	float Distance;
};

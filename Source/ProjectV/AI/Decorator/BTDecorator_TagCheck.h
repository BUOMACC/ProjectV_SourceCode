// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "GameplayTagContainer.h"
#include "BTDecorator_TagCheck.generated.h"

/*
 *	선택한 Blackboard Key의 Actor가 Tag중 하나라도 가지고있는지 체크
 */
UCLASS()
class PROJECTV_API UBTDecorator_TagCheck : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTDecorator_TagCheck();

private:
	virtual FString GetStaticDescription() const override;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Tag Check")
	FGameplayTagContainer TagContainer;
};

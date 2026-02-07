// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_ChanceOf.generated.h"

/*
 *	활성화되는 동안 랜덤한 숫자를 뽑아 Chance에 해당되는지 체크
 *	※ 여러틱에 호출하지 말고, 단발성으로 호출할것
 */
UCLASS()
class PROJECTV_API UBTDecorator_ChanceOf : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_ChanceOf();

private:
	virtual FString GetStaticDescription() const override;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

protected:
	UPROPERTY(EditAnywhere, Category = "Chance Of")
	int32 Chance;

};

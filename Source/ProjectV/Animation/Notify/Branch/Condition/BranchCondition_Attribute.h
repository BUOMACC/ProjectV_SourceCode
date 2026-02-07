// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BranchCondition_Base.h"
#include "ProjectVCommonEnums.h"
#include "BranchCondition_Attribute.generated.h"

/**
 *	Attribute 값이 원하는 조건에 부합한지 체크합니다.
 *	(Attribute) (Operator) (Value) 순서로 체크합니다.
 */
UCLASS()
class PROJECTV_API UBranchCondition_Attribute : public UBranchCondition_Base
{
	GENERATED_BODY()
	
public:
	virtual bool CheckCondition(ACombatCharacter* Instigator) override;

protected:
	UPROPERTY(EditDefaultsOnly)
	FGameplayAttribute LeftOperandAttribute;

	UPROPERTY(EditDefaultsOnly)
	EComparisonOperator Operator;

	UPROPERTY(EditDefaultsOnly)
	bool bUseAttributeValue;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bUseAttributeValue == true", EditConditionHides))
	FGameplayAttribute RightOperandAttribute;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bUseAttributeValue == false", EditConditionHides))
	float RightOperandValue;
};

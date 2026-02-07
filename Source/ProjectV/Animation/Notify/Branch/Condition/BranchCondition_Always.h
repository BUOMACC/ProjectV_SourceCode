// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BranchCondition_Base.h"
#include "BranchCondition_Always.generated.h"

/**
 *	항상 실행하는 조건입니다.
 */
UCLASS(DisplayName = "BranchCondition_Always")
class PROJECTV_API UBranchCondition_Always : public UBranchCondition_Base
{
	GENERATED_BODY()
	
public:
	virtual bool CheckCondition(ACombatCharacter* Instigator) override;
};

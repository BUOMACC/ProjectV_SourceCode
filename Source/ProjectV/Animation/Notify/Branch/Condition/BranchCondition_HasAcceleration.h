// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BranchCondition_Base.h"
#include "BranchCondition_HasAcceleration.generated.h"

/**
 *  가속이 있는지 체크합니다.
 */
UCLASS(DisplayName = "BranchCondition_HasAcceleration")
class PROJECTV_API UBranchCondition_HasAcceleration : public UBranchCondition_Base
{
	GENERATED_BODY()

public:
	virtual bool CheckCondition(ACombatCharacter* Instigator) override;
};

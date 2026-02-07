// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BranchCondition_Base.h"
#include "BranchCondition_IsAir.generated.h"

/**
 *	공중에 있는지 체크합니다.
 */
UCLASS()
class PROJECTV_API UBranchCondition_IsAir : public UBranchCondition_Base
{
	GENERATED_BODY()
	
public:
	virtual bool CheckCondition(ACombatCharacter* Instigator) override;
};

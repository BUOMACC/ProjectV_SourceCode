// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Character/CombatCharacter.h"
#include "BranchAction_Base.generated.h"

/**
 *	Branch 조건 만족시 실행되는 Action Base
 */
UCLASS(Abstract, EditInlineNew, DisplayName = "BranchAction_None")
class PROJECTV_API UBranchAction_Base : public UObject
{
	GENERATED_BODY()
	
public:
	UBranchAction_Base();
	virtual void Activate(ACombatCharacter* Instigator);
};

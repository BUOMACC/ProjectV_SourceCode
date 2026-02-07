// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Character/CombatCharacter.h"
#include "BranchCondition_Base.generated.h"

/**
 *	조건을 체크해 결과를 반환하는 Branch Condition Base
 *	해당 클래스를 상속받아 조건을 구현하여 사용함
 * 
 *	 ※ 파생 클래스의 UCLASS 메타 지정자로 "Instanced"를 추가해야함
 */
UCLASS(Abstract, EditInlineNew, DisplayName = "Condition_None")
class PROJECTV_API UBranchCondition_Base : public UObject
{
	GENERATED_BODY()
	
public:
	UBranchCondition_Base();
	virtual bool CheckCondition(ACombatCharacter* Instigator);

public:
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "결과를 반전할까요?"))
	bool bReverseResult;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BranchCondition_Base.h"
#include "BranchCondition_InputBufferIsSet.generated.h"

/**
 *	일치하는 태그가 입력버퍼에 입력되어 있는지 체크합니다.
 */
UCLASS(DisplayName = "BranchCondition_InputBufferIsSet")
class PROJECTV_API UBranchCondition_InputBufferIsSet : public UBranchCondition_Base
{
	GENERATED_BODY()

public:
	virtual bool CheckCondition(ACombatCharacter* Instigator) override;

protected:
	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Input.Type"))
	FGameplayTag InputTag;
};

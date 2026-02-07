// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemFragmentBase.h"
#include "ItemFragment_LimitStack.generated.h"

/**
 *	아이템의 개수에 관련된 정보를 설정합니다.
 */
UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class PROJECTV_API UItemFragment_LimitStack : public UItemFragmentBase
{
	GENERATED_BODY()
	
public:
	UItemFragment_LimitStack();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Action")
	int32 MaxStackCount;
};

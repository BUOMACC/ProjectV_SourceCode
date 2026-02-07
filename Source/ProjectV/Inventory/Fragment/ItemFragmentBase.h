// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemFragmentBase.generated.h"

class UItemInstance;

/**
 *	Item Data(Data Asset)의 세부 데이터 지정 오브젝트 베이스
 */
UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew)
class PROJECTV_API UItemFragmentBase : public UObject
{
	GENERATED_BODY()
	
public:
	UItemFragmentBase();

public:
	virtual void OnInstanceCreated(UItemInstance* InItemInstance);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemFragmentBase.h"
#include "GameplayTagContainer.h"
#include "Inventory/QuickSlotComponent.h"
#include "ItemFragment_QuickSlot.generated.h"

/**
 *	아이템을 퀵슬롯에 장착하기 위해 필요한 정보를 설정합니다.
 *	해당 Fragment가 추가되면 퀵슬롯에 장착할 수 있습니다.
 */
UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class PROJECTV_API UItemFragment_QuickSlot : public UItemFragmentBase
{
	GENERATED_BODY()
	
public:
	UItemFragment_QuickSlot();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quick Slot")
	EQuickSlotType SlotType;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemFragment_QuickSlot.h"
#include "ItemFragment_Equipment.generated.h"

class UEquipmentDefinition;

/**
 *	아이템을 장착했을때 정보를 설정합니다.
 */
UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class PROJECTV_API UItemFragment_Equipment : public UItemFragment_QuickSlot
{
	GENERATED_BODY()
	
public:
	UItemFragment_Equipment();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quick Slot|Equipment")
	TSubclassOf<UEquipmentDefinition> EquipmentDefinition;
};

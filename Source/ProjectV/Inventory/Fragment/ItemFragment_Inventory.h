// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemFragmentBase.h"
#include "GameplayTagContainer.h"
#include "ItemFragment_Inventory.generated.h"

class UTexture2D;

UENUM(BlueprintType)
enum class EItemTier : uint8
{
	Normal,
	Rare,
	Epic,
	Legendary
};

/**
 *	아이템이 Inventory 내에 있을때 필요한 정보를 설정합니다.
 *	해당 Fragment가 추가되면 인벤토리에 표시됩니다.
 */
UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class PROJECTV_API UItemFragment_Inventory : public UItemFragmentBase
{
	GENERATED_BODY()
	
public:
	UItemFragment_Inventory();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Slot")
	TObjectPtr<UTexture2D> DisplayIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Slot")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Slot", meta = (MultiLine = "true"))
	FText DisplayDescription;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Slot", meta = (Categories = "Item.Type"))
	//FGameplayTag ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Slot")
	EItemTier ItemTier;
};

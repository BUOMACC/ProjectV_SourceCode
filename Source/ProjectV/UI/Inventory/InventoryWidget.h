// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CombatActivatableWidget.h"
#include "InventoryWidget.generated.h"

class UInventoryComponent;
class UInventoryBagWidget;
class UItemDetailWidget;

/**
 *	인벤토리를 관리하는 탭 컨텐츠를 표시하는 위젯입니다.
 */
UCLASS()
class PROJECTV_API UInventoryWidget : public UCombatActivatableWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnActivated() override;

	UFUNCTION()
	void OnItemEntryHighlightStateChanged(bool bIsHighlighted, UItemInstance* ItemInstance);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Inventory Widget", meta = (BindWidget))
	TObjectPtr<UInventoryBagWidget> InventoryBagWidget;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Widget", meta = (BindWidget))
	TObjectPtr<UItemDetailWidget> ItemDetailWidget;
};

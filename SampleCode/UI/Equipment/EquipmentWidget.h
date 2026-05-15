// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CombatActivatableWidget.h"
#include "EquipmentWidget.generated.h"

class UWidgetSwitcher;
class UEquipmentSlotListWidget;
class UEquipmentSlotWidget;
class UInventoryBagWidget;
class UItemDetailWidget;
class UInventoryComponent;

enum class EEquipmentWidgetContent : uint8
{
	Equipment,
	Inventory,
};

/**
 *	장착 아이템을 관리하는 탭 컨텐츠를 표시하는 위젯입니다.
 */
UCLASS()
class PROJECTV_API UEquipmentWidget : public UCombatActivatableWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	virtual void NativeOnActivated() override;

protected:
	void BindSlotEvent(TArray<UEquipmentSlotWidget*> SlotWidgets);
	void SetContent(EEquipmentWidgetContent ContnetType);

protected:
	UFUNCTION()
	void OnEquipmentSlotClicked(UEquipmentSlotWidget* InSlotWidget, const FPointerEvent& InMouseEvent);

	UFUNCTION()
	void HandleUpdateItemDetail(bool bIsHighlighted, UItemInstance* ItemInstance);

	UFUNCTION()
	void OnItemEntryClicked(UItemEntryWidget* SlotWidget);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Equipment Widget", meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher_Content;

	UPROPERTY(BlueprintReadOnly, Category = "Equipment Widget", meta = (BindWidget))
	TObjectPtr<UWidget> Content_Equipment;

	UPROPERTY(BlueprintReadOnly, Category = "Equipment Widget", meta = (BindWidget))
	TObjectPtr<UWidget> Content_Inventory;

	UPROPERTY(BlueprintReadOnly, Category = "Equipment Widget", meta = (BindWidget))
	TObjectPtr<UInventoryBagWidget> InventoryBagWidget;

	UPROPERTY(BlueprintReadOnly, Category = "Equipment Widget", meta = (BindWidget))
	TObjectPtr<UEquipmentSlotListWidget> SlotList_Consumption;

	UPROPERTY(BlueprintReadOnly, Category = "Equipment Widget", meta = (BindWidget))
	TObjectPtr<UEquipmentSlotListWidget> SlotList_Skill;

	UPROPERTY(BlueprintReadOnly, Category = "Equipment Widget", meta = (BindWidget))
	TObjectPtr<UEquipmentSlotListWidget> SlotList_Weapon;

	UPROPERTY(BlueprintReadOnly, Category = "Equipment Widget", meta = (BindWidget))
	TObjectPtr<UItemDetailWidget> ItemDetailWidget;

private:
	/* 현재 선택대상으로 지정된 슬롯입니다. */
	TObjectPtr<UEquipmentSlotWidget> CurrentEquipmentSlotWidget;
};

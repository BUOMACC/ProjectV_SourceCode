// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/QuickSlotComponent.h"
#include "QuickSlotHUDWidget.generated.h"

class UItemInstance;
class UQuickSlotWidget;

/**
 *	인게임 HUD에 표시되는 퀵슬롯을 관리하는 위젯입니다.
 */
UCLASS()
class PROJECTV_API UQuickSlotHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UQuickSlotHUDWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	UFUNCTION(BlueprintCallable, Category = "QuickSlot HUD Widget")
	void InitWidget(UInventoryComponent* InventoryComponent, UQuickSlotComponent* QuickSlotComponent);

protected:
	UFUNCTION()
	void OnInventoryItemChanged(UItemInstance* ItemInstance, int32 OldCount, int32 NewCount);

	UFUNCTION()
	void OnQuickSlotChanged(EQuickSlotType InSlotType, int32 InSlotIndex, UItemInstance* InItemInstance);

	UFUNCTION()
	void OnQuickSlotActiveIndexChanged(EQuickSlotType InSlotType, int32 InNewSlotIndex);

protected:
	UQuickSlotWidget* FindSlot(UItemInstance* InItemInstance);
	UQuickSlotWidget* FindSlot(EQuickSlotType InSlotType);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "QuickSlot HUD Widget", meta = (BindWidget))
	TObjectPtr<UQuickSlotWidget> QuickSlot_Weapon;

	UPROPERTY(BlueprintReadOnly, Category = "QuickSlot HUD Widget", meta = (BindWidget))
	TObjectPtr<UQuickSlotWidget> QuickSlot_Skill;

	UPROPERTY(BlueprintReadOnly, Category = "QuickSlot HUD Widget", meta = (BindWidget))
	TObjectPtr<UQuickSlotWidget> QuickSlot_Consumption;

private:
	UPROPERTY()
	TArray<TObjectPtr<UQuickSlotWidget>> SlotWidgets;
};

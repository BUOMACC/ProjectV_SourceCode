// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipmentSlotListWidget.generated.h"

class UInventoryComponent;
class UQuickSlotComponent;
class UEquipmentSlotWidget;
class UHorizontalBox;

/**
 *	EquipmentSlot들을 그룹단위로 묶어 관리하는 위젯입니다.
 */
UCLASS()
class PROJECTV_API UEquipmentSlotListWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UEquipmentSlotListWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	TArray<UEquipmentSlotWidget*> CreateSlots(UInventoryComponent* InventoryComponent, UQuickSlotComponent* QuickSlotComponent);

	/* 입력받은 인덱스에 해당하는 슬롯을 반환합니다. */
	UEquipmentSlotWidget* GetSlotAt(int32 Index);

	/* 입력받은 아이템을 장착중인 슬롯을 반환합니다. */
	UEquipmentSlotWidget* FindSlot(UItemInstance* ItemInstance);

protected:
	UFUNCTION()
	void OnInventoryItemChanged(UItemInstance* ItemInstance, int32 OldCount, int32 NewCount);

	UFUNCTION()
	void OnQuickSlotChanged(EQuickSlotType InSlotType, int32 InSlotIndex, UItemInstance* InItemInstance);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Equipment Slot List Widget", meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox_Slots;

public:
	/* 해당 그룹의 슬롯들이 사용하는 타입입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Slot List Widget")
	EQuickSlotType SlotType;

	/* 생성될 위젯 클래스입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Slot List Widget")
	TSubclassOf<UEquipmentSlotWidget> SlotWidgetClass;

#if WITH_EDITORONLY_DATA
	/* * 에디터 전용 * 디자인 타임에 보여질 슬롯 수를 지정합니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Slot List Widget")
	int32 PreviewSlotCount;
#endif

private:
	UPROPERTY()
	TObjectPtr<UQuickSlotComponent> CachedQuickSlotComponent;

	UPROPERTY()
	TObjectPtr<UInventoryComponent> CachedInventoryComponent;
};

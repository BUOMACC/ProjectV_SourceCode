// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSlotListWidget.h"
#include "EquipmentSlotWidget.h"
#include "Equipment/EquipmentComponent.h"
#include "Components/HorizontalBox.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/ItemInstance.h"

UEquipmentSlotListWidget::UEquipmentSlotListWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SlotType = EQuickSlotType::None;
}

void UEquipmentSlotListWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

#if WITH_EDITOR
	if (IsDesignTime())
	{
		HorizontalBox_Slots->ClearChildren();

		for (int32 i = 0; i < PreviewSlotCount; i++)
		{
			UEquipmentSlotWidget* SlotWidget = CreateWidget<UEquipmentSlotWidget>(this, SlotWidgetClass);
			HorizontalBox_Slots->AddChildToHorizontalBox(SlotWidget);
		}
	}
#endif
}

void UEquipmentSlotListWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UEquipmentSlotListWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (CachedQuickSlotComponent)
	{
		CachedQuickSlotComponent->OnQuickSlotChanged.RemoveDynamic(this, &UEquipmentSlotListWidget::OnQuickSlotChanged);
		CachedQuickSlotComponent = nullptr;
	}

	if (CachedInventoryComponent)
	{
		CachedInventoryComponent->OnInventoryItemChanged.RemoveDynamic(this, &UEquipmentSlotListWidget::OnInventoryItemChanged);
		CachedInventoryComponent = nullptr;
	}
}

TArray<UEquipmentSlotWidget*> UEquipmentSlotListWidget::CreateSlots(UInventoryComponent* InventoryComponent, UQuickSlotComponent* QuickSlotComponent)
{
	TArray<UEquipmentSlotWidget*> Result;
	Result.Empty();

	if (SlotType == EQuickSlotType::None || SlotWidgetClass == nullptr)
		return Result;

	if (QuickSlotComponent)
	{
		CachedQuickSlotComponent = QuickSlotComponent;

		HorizontalBox_Slots->ClearChildren();

		if (FQuickSlot* QuickSlot = QuickSlotComponent->GetQuickSlot(SlotType))
		{
			// Create Slot Widget, Load Slot Data
			for (int32 i = 0; i < QuickSlot->GetSlotSize(); i++)
			{
				UEquipmentSlotWidget* SlotWidget = CreateWidget<UEquipmentSlotWidget>(GetOwningPlayer(), SlotWidgetClass);
				SlotWidget->Init(SlotType, i);

				// 퀵슬롯 정보가 있다면 로드
				if (UItemInstance* InitialItem = QuickSlotComponent->GetQuickSlotItem(SlotType, i))
				{
					SlotWidget->UpdateWidget(InitialItem, false);
				}

				HorizontalBox_Slots->AddChildToHorizontalBox(SlotWidget);
				Result.Add(SlotWidget);
			}
		}

		QuickSlotComponent->OnQuickSlotChanged.AddDynamic(this, &UEquipmentSlotListWidget::OnQuickSlotChanged);
	}

	if (InventoryComponent)
	{
		CachedInventoryComponent = InventoryComponent;
		InventoryComponent->OnInventoryItemChanged.AddDynamic(this, &UEquipmentSlotListWidget::OnInventoryItemChanged);
	}

	return Result;
}

UEquipmentSlotWidget* UEquipmentSlotListWidget::GetSlotAt(int32 Index)
{
	if (UWidget* Widget = HorizontalBox_Slots->GetChildAt(Index))
	{
		if (UEquipmentSlotWidget* SlotWidget = Cast<UEquipmentSlotWidget>(Widget))
		{
			return SlotWidget;
		}
	}
	return nullptr;
}

UEquipmentSlotWidget* UEquipmentSlotListWidget::FindSlot(UItemInstance* ItemInstance)
{
	for (UWidget* Widget : HorizontalBox_Slots->GetAllChildren())
	{
		if (UEquipmentSlotWidget* SlotWidget = Cast<UEquipmentSlotWidget>(Widget))
		{
			if (SlotWidget->GetSlotItem() == ItemInstance)
			{
				return SlotWidget;
			}
		}
	}
	return nullptr;
}

void UEquipmentSlotListWidget::OnInventoryItemChanged(UItemInstance* ItemInstance, int32 OldCount, int32 NewCount)
{
	/* 이벤트로 호출된 아이템을 장착중인 슬롯에만 해당 */
	if (UEquipmentSlotWidget* SlotWidget = FindSlot(ItemInstance))
	{
		if (NewCount <= 0)
		{
			SlotWidget->ClearWidget();
			return;
		}
		SlotWidget->UpdateWidget(ItemInstance);
	}
}

void UEquipmentSlotListWidget::OnQuickSlotChanged(EQuickSlotType InSlotType, int32 InSlotIndex, UItemInstance* InItemInstance)
{
	if (InSlotType == SlotType)
	{
		if (UEquipmentSlotWidget* SlotWidget = GetSlotAt(InSlotIndex))
		{
			if (!IsValid(InItemInstance))
			{
				SlotWidget->ClearWidget();
				return;
			}
			SlotWidget->UpdateWidget(InItemInstance, true);
		}
	}
}

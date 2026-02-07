// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickSlotHUDWidget.h"
#include "Inventory/ItemInstance.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/Fragment/ItemFragment_QuickSlot.h"
#include "QuickSlotWidget.h"

UQuickSlotHUDWidget::UQuickSlotHUDWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UQuickSlotHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SlotWidgets.Empty();
	SlotWidgets.Add(QuickSlot_Consumption);
	SlotWidgets.Add(QuickSlot_Weapon);
	SlotWidgets.Add(QuickSlot_Skill);
}

void UQuickSlotHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UQuickSlotHUDWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UQuickSlotHUDWidget::InitWidget(UInventoryComponent* InventoryComponent, UQuickSlotComponent* QuickSlotComponent)
{
	if (InventoryComponent && QuickSlotComponent)
	{
		InventoryComponent->OnInventoryItemChanged.AddDynamic(this, &UQuickSlotHUDWidget::OnInventoryItemChanged);

		QuickSlotComponent->OnQuickSlotChanged.AddDynamic(this, &UQuickSlotHUDWidget::OnQuickSlotChanged);
		QuickSlotComponent->OnQuickSlotActiveIndexChanged.AddDynamic(this, &UQuickSlotHUDWidget::OnQuickSlotActiveIndexChanged);

		// UI가 보여지는 타이밍에 현재 데이터 최초 동기화
		for (UQuickSlotWidget* SlotWidget : SlotWidgets)
		{
			SlotWidget->InitWidget(QuickSlotComponent);

			if (FQuickSlot* QuickSlot = QuickSlotComponent->GetQuickSlot(SlotWidget->GetSlotType()))
			{
				SlotWidget->SetSlotIndex(QuickSlot->ActiveSlotIndex, false, false);
			}
		}
	}

}

void UQuickSlotHUDWidget::OnInventoryItemChanged(UItemInstance* ItemInstance, int32 OldCount, int32 NewCount)
{
	if (UQuickSlotWidget* SlotWidget = FindSlot(ItemInstance))
	{
		SlotWidget->UpdateWidget();
	}
}

void UQuickSlotHUDWidget::OnQuickSlotChanged(EQuickSlotType InSlotType, int32 InSlotIndex, UItemInstance* InItemInstance)
{
	if (UQuickSlotWidget* SlotWidget = FindSlot(InSlotType))
	{
		SlotWidget->UpdateWidget();
	}
}

void UQuickSlotHUDWidget::OnQuickSlotActiveIndexChanged(EQuickSlotType InSlotType, int32 InNewSlotIndex)
{
	if (UQuickSlotWidget* SlotWidget = FindSlot(InSlotType))
	{
		SlotWidget->SetSlotIndex(InNewSlotIndex, true, true);
	}
}

UQuickSlotWidget* UQuickSlotHUDWidget::FindSlot(UItemInstance* ItemInstance)
{
	for (UQuickSlotWidget* SlotWidget : SlotWidgets)
	{
		if (SlotWidget->GetSlotItem() == ItemInstance)
		{
			return SlotWidget;
		}
	}
	return nullptr;
}

UQuickSlotWidget* UQuickSlotHUDWidget::FindSlot(EQuickSlotType InSlotType)
{
	for (UQuickSlotWidget* SlotWidget : SlotWidgets)
	{
		if (SlotWidget->GetSlotType() == InSlotType)
		{
			return SlotWidget;
		}
	}
	return nullptr;
}

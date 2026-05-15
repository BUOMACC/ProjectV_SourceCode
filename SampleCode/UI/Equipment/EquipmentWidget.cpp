// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentWidget.h"
#include "EquipmentSlotListWidget.h"
#include "EquipmentSlotWidget.h"
#include "Components/WidgetSwitcher.h"
#include "UI/Inventory/InventoryBagWidget.h"
#include "UI/Inventory/ItemDetailWidget.h"
#include "UI/Inventory/ItemEntryWidget.h"
#include "Inventory/QuickSlotComponent.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/ItemInstance.h"

void UEquipmentWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UEquipmentWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (APawn* Pawn = GetOwningPlayerPawn())
	{
		UInventoryComponent* InventoryComponent = Pawn->GetComponentByClass<UInventoryComponent>();
		if (InventoryComponent)
		{
			for (UItemEntryWidget* EntryWidget : InventoryBagWidget->LoadItems(InventoryComponent))
			{
				EntryWidget->OnItemEntryHighlightStateChanged.AddDynamic(this, &UEquipmentWidget::HandleUpdateItemDetail);
				EntryWidget->OnItemEntryClicked.AddDynamic(this, &UEquipmentWidget::OnItemEntryClicked);
			}
		}

		UQuickSlotComponent* QuickSlotComponent = Pawn->GetComponentByClass<UQuickSlotComponent>();
		if (QuickSlotComponent)
		{
			BindSlotEvent(SlotList_Consumption->CreateSlots(InventoryComponent, QuickSlotComponent));
			BindSlotEvent(SlotList_Skill->CreateSlots(InventoryComponent, QuickSlotComponent));
			BindSlotEvent(SlotList_Weapon->CreateSlots(InventoryComponent, QuickSlotComponent));
		}
	}
}

void UEquipmentWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	CurrentEquipmentSlotWidget = nullptr;
	WidgetSwitcher_Content->SetActiveWidget(Content_Equipment);
}

void UEquipmentWidget::BindSlotEvent(TArray<UEquipmentSlotWidget*> SlotWidgets)
{
	for (UEquipmentSlotWidget* SlotWidget : SlotWidgets)
	{
		SlotWidget->OnEquipmentSlotHighlightStateChanged.AddDynamic(this, &UEquipmentWidget::HandleUpdateItemDetail);
		SlotWidget->OnEquipmentSlotClicked.AddDynamic(this, &UEquipmentWidget::OnEquipmentSlotClicked);
	}
}

void UEquipmentWidget::SetContent(EEquipmentWidgetContent ContnetType)
{
	if (WidgetSwitcher_Content == nullptr)
		return;

	switch (ContnetType)
	{
	case EEquipmentWidgetContent::Equipment:
		WidgetSwitcher_Content->SetActiveWidget(Content_Equipment);
		break;

	case EEquipmentWidgetContent::Inventory:
		WidgetSwitcher_Content->SetActiveWidget(Content_Inventory);
		break;
	}
}

void UEquipmentWidget::OnEquipmentSlotClicked(UEquipmentSlotWidget* InSlotWidget, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		CurrentEquipmentSlotWidget = InSlotWidget;
		SetContent(EEquipmentWidgetContent::Inventory);
	}
	else if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		if (APawn* Pawn = GetOwningPlayerPawn())
		{
			if (UQuickSlotComponent* QuickSlotComponent = Pawn->GetComponentByClass<UQuickSlotComponent>())
			{
				QuickSlotComponent->RemoveFromQuickSlot(InSlotWidget->GetSlotType(), InSlotWidget->GetSlotIndex());
				ItemDetailWidget->ClearDetail();
			}
		}
	}
}

void UEquipmentWidget::HandleUpdateItemDetail(bool bIsHighlighted, UItemInstance* ItemInstance)
{
	if (ItemDetailWidget)
	{
		if (bIsHighlighted)
		{
			ItemDetailWidget->UpdateWidget(ItemInstance);
		}
		else
		{
			ItemDetailWidget->ClearDetail();
		}
	}
}

void UEquipmentWidget::OnItemEntryClicked(UItemEntryWidget* SlotWidget)
{
	if (APawn* Pawn = GetOwningPlayerPawn())
	{
		if (UQuickSlotComponent* QuickSlotComponent = Pawn->GetComponentByClass<UQuickSlotComponent>())
		{
			QuickSlotComponent->AddToQuickSlot(SlotWidget->GetSlotItem(), CurrentEquipmentSlotWidget->GetSlotType(), CurrentEquipmentSlotWidget->GetSlotIndex());
		}
	}

	CurrentEquipmentSlotWidget = nullptr;
	SetContent(EEquipmentWidgetContent::Equipment);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/ItemInstance.h"
#include "InventoryBagWidget.h"
#include "ItemEntryWidget.h"
#include "ItemDetailWidget.h"

void UInventoryWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	if (APawn* Pawn = GetOwningPlayerPawn())
	{
		if (UInventoryComponent* InventoryComponent = Pawn->GetComponentByClass<UInventoryComponent>())
		{
			for (UItemEntryWidget* EntryWidget : InventoryBagWidget->LoadItems(InventoryComponent))
			{
				EntryWidget->OnItemEntryHighlightStateChanged.AddDynamic(this, &UInventoryWidget::OnItemEntryHighlightStateChanged);

			}
		}
	}
}

void UInventoryWidget::OnItemEntryHighlightStateChanged(bool bIsHighlighted, UItemInstance* ItemInstance)
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

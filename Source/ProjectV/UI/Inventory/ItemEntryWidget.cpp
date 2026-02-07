// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemEntryWidget.h"
#include "ItemSlotWidget.h"
#include "Inventory/ItemInstance.h"
#include "Inventory/Fragment/ItemFragment_Inventory.h"

#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Sound/SoundBase.h"

UItemEntryWidget::UItemEntryWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bHighlighted = false;
}

void UItemEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UItemEntryWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	SetHighlight(true);
}

void UItemEntryWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	SetHighlight(false);
}

FReply UItemEntryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Result = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	OnItemEntryClicked.Broadcast(this);
	return Result;
}

void UItemEntryWidget::UpdateWidget(UItemInstance* ItemInstance)
{
	if (!IsValid(ItemInstance))
	{
		return;
	}

	CachedItemInstance = ItemInstance;

	if (ItemSlotWidget)
	{
		ItemSlotWidget->UpdateWidget(ItemInstance);
	}

	if (const UItemFragment_Inventory* InventoryFragment = ItemInstance->FindItemFragmentByClass<UItemFragment_Inventory>())
	{
		Text_Name_Default->SetText(InventoryFragment->DisplayName);
		Text_Name_Highlight->SetText(InventoryFragment->DisplayName);
	}
	else
	{
		Text_Name_Default->SetText(FText::GetEmpty());
		Text_Name_Highlight->SetText(FText::GetEmpty());
	}
}

void UItemEntryWidget::SetHighlight(bool bHighlight)
{
	if (bHighlighted != bHighlight)
	{
		bHighlighted = bHighlight;
		
		UWidget* HighlightWidget = (bHighlight) ? Border_Name_Highlight : Border_Name_Default;
		if (HighlightWidget)
		{
			WidgetSwitcher_Name->SetActiveWidget(HighlightWidget);
		}

		if (bHighlighted)
		{
			PlaySound(HighlightSound);
		}

		OnItemEntryHighlightStateChanged.Broadcast(bHighlighted, CachedItemInstance);
	}
}

UItemInstance* UItemEntryWidget::GetSlotItem() const
{
	return CachedItemInstance;
}

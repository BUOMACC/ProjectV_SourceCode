// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Overlay.h"
#include "Inventory/ItemInstance.h"
#include "Inventory/Fragment/ItemFragment_Inventory.h"
#include "Kismet/KismetTextLibrary.h"

UItemSlotWidget::UItemSlotWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UItemSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UItemSlotWidget::UpdateWidget(UItemInstance* ItemInstance)
{
	SetIcon(ItemInstance);
	SetGrade(ItemInstance);
	SetCount(ItemInstance);
}

void UItemSlotWidget::SetIcon(UItemInstance* ItemInstance)
{
	if (!IsValid(ItemInstance))
	{
		Image_Icon->SetVisibility(ESlateVisibility::Collapsed);
		Image_Icon->SetBrushFromTexture(nullptr);
		return;
	}

	if (const UItemFragment_Inventory* InventoryFragment = ItemInstance->FindItemFragmentByClass<UItemFragment_Inventory>())
	{
		Image_Icon->SetVisibility(ESlateVisibility::HitTestInvisible);
		Image_Icon->SetBrushFromTexture(InventoryFragment->DisplayIcon, false);
	}
	else
	{
		Image_Icon->SetVisibility(ESlateVisibility::Collapsed);
		Image_Icon->SetBrushFromTexture(nullptr);
	}
}

void UItemSlotWidget::SetGrade(UItemInstance* ItemInstance)
{
	if (!IsValid(ItemInstance))
	{
		WidgetSwitcher_Tier->SetActiveWidget(Overlay_Tier_Normal);
		return;
	}

	if (const UItemFragment_Inventory* InventoryFragment = ItemInstance->FindItemFragmentByClass<UItemFragment_Inventory>())
	{
		switch (InventoryFragment->ItemTier)
		{
			case EItemTier::Normal:
				WidgetSwitcher_Tier->SetActiveWidget(Overlay_Tier_Normal);
				break;

			case EItemTier::Rare:
				WidgetSwitcher_Tier->SetActiveWidget(Overlay_Tier_Rare);
				break;

			case EItemTier::Epic:
				WidgetSwitcher_Tier->SetActiveWidget(Overlay_Tier_Epic);
				break;

			case EItemTier::Legendary:
				WidgetSwitcher_Tier->SetActiveWidget(Overlay_Tier_Legendary);
				break;
		}
	}
}

void UItemSlotWidget::SetCount(UItemInstance* ItemInstance)
{
	if (!IsValid(ItemInstance) || ItemInstance->GetCount() <= 0)
	{
		Text_Count->SetVisibility(ESlateVisibility::Collapsed);
		Text_Count->SetText(FText::GetEmpty());
		return;
	}

	Text_Count->SetVisibility(ESlateVisibility::HitTestInvisible);
	Text_Count->SetText(FText::AsNumber(ItemInstance->GetCount()));
}

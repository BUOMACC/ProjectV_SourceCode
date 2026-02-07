// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Inventory/ItemInstance.h"
#include "Inventory/Fragment/ItemFragment_Inventory.h"

UEquipmentSlotWidget::UEquipmentSlotWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CachedItemInstance = nullptr;
	SlotType = EQuickSlotType::None;
	SlotIndex = 0;
	bHighlighted = false;
}

void UEquipmentSlotWidget::Init(EQuickSlotType InSlotType, int32 InSlotIndex)
{
	SlotType = InSlotType;
	SlotIndex = InSlotIndex;

	ClearWidget();
}

void UEquipmentSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UEquipmentSlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	
	SetHighlight(true);
}

void UEquipmentSlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	
	SetHighlight(false);
}

FReply UEquipmentSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Result = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	OnEquipmentSlotClicked.Broadcast(this, InMouseEvent);
	return Result;
}

void UEquipmentSlotWidget::UpdateWidget(UItemInstance* ItemInstance, bool bPlaySound)
{
	CachedItemInstance = ItemInstance;

	SetIcon(ItemInstance);
	SetCount(ItemInstance);

	if (bPlaySound)
	{
		USoundBase* SoundToPlay = (IsValid(ItemInstance) ? SlotEquipSound : SlotUnEquipSound);
		PlaySound(SoundToPlay);
	}
}

void UEquipmentSlotWidget::ClearWidget()
{
	CachedItemInstance = nullptr;

	SetIcon(nullptr);
	SetCount(nullptr);
}

UItemInstance* UEquipmentSlotWidget::GetSlotItem() const
{
	return CachedItemInstance;
}

EQuickSlotType UEquipmentSlotWidget::GetSlotType() const
{
	return SlotType;
}

int32 UEquipmentSlotWidget::GetSlotIndex() const
{
	return SlotIndex;
}

void UEquipmentSlotWidget::SetHighlight(bool bHighlight)
{
	if (bHighlighted != bHighlight)
	{
		bHighlighted = bHighlight;

		ESlateVisibility NewVisibility = (bHighlight) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;
		Image_Highlight->SetVisibility(NewVisibility);

		if (bHighlighted)
		{
			PlaySound(HighlightSound);
		}

		OnEquipmentSlotHighlightStateChanged.Broadcast(bHighlighted, CachedItemInstance);
	}
}

void UEquipmentSlotWidget::SetIcon(UItemInstance* ItemInstance)
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

void UEquipmentSlotWidget::SetCount(UItemInstance* ItemInstance)
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


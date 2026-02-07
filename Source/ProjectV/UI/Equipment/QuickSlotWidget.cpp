// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickSlotWidget.h"
#include "Inventory/ItemInstance.h"
#include "Inventory/Fragment/ItemFragment_Inventory.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

UQuickSlotWidget::UQuickSlotWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CachedItemInstance = nullptr;
	SlotType = EQuickSlotType::None;
	bShowCountText = true;
	SlotIndex = 0;
}

void UQuickSlotWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

#if WITH_EDITOR
	if (IsDesignTime())
	{
		Text_Count->SetRenderOpacity(bShowCountText);
	}
#endif
}

void UQuickSlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Text_Count->SetRenderOpacity(bShowCountText);
}

void UQuickSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UQuickSlotWidget::InitWidget(UQuickSlotComponent* QuickSlotComponent)
{
	CachedQuickSlotComponent = QuickSlotComponent;
}

void UQuickSlotWidget::UpdateWidget()
{
	if (!CachedQuickSlotComponent)
	{
		ClearWidget();
		return;
	}

	CachedItemInstance = CachedQuickSlotComponent->GetQuickSlotItem(SlotType, SlotIndex);

	if (!IsValid(CachedItemInstance) || CachedItemInstance->GetCount() <= 0)
	{
		ClearWidget();
		return;
	}

	SetIcon(CachedItemInstance);
	SetCount(CachedItemInstance);
	SetName(CachedItemInstance);
}

void UQuickSlotWidget::ClearWidget()
{
	CachedItemInstance = nullptr;

	SetIcon(nullptr);
	SetCount(nullptr);
	SetName(nullptr);
}

UItemInstance* UQuickSlotWidget::GetSlotItem() const
{
	return CachedItemInstance;
}

EQuickSlotType UQuickSlotWidget::GetSlotType() const
{
	return SlotType;
}

void UQuickSlotWidget::SetSlotIndex(int32 NewSlotIndex, bool bPlaySound, bool bPlayAnimation)
{
	SlotIndex = NewSlotIndex;

	UpdateWidget();

	if (bPlaySound && SlotSound)
	{
		PlaySound(SlotSound);
	}

	if (bPlayAnimation)
	{
		if (SwitchAnimation)
		{
			PlayAnimation(SwitchAnimation);
		}

		if (NameAnimation && CachedItemInstance)
		{
			PlayAnimation(NameAnimation);
		}
	}
}

int32 UQuickSlotWidget::GetSlotIndex() const
{
	return SlotIndex;
}

void UQuickSlotWidget::SetIcon(UItemInstance* ItemInstance)
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

void UQuickSlotWidget::SetCount(UItemInstance* ItemInstance)
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

void UQuickSlotWidget::SetName(UItemInstance* ItemInstance)
{
	if (!IsValid(ItemInstance))
	{
		return;
	}

	if (const UItemFragment_Inventory* InventoryFragment = ItemInstance->FindItemFragmentByClass<UItemFragment_Inventory>())
	{
		Text_Name->SetText(InventoryFragment->DisplayName);
	}
}

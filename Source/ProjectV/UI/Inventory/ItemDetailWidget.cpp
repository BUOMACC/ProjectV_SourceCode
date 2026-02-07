// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDetailWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Inventory/ItemInstance.h"
#include "Inventory/Fragment/ItemFragment_Inventory.h"

UItemDetailWidget::UItemDetailWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UItemDetailWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ClearDetail();
}

void UItemDetailWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UItemDetailWidget::UpdateWidget(UItemInstance* ItemInstance)
{
	if (ItemInstance == nullptr)
	{
		ClearDetail();
		return;
	}

	const UItemFragment_Inventory* InventoryFragment = ItemInstance->FindItemFragmentByClass<UItemFragment_Inventory>();
	if (InventoryFragment == nullptr)
	{
		ClearDetail();
		return;
	}

	Image_Icon->SetBrushFromTexture(InventoryFragment->DisplayIcon);
	Image_Icon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	Text_Name->SetText(InventoryFragment->DisplayName);
	Text_Description->SetText(InventoryFragment->DisplayDescription);
}

void UItemDetailWidget::ClearDetail()
{
	Image_Icon->SetBrushFromTexture(nullptr);
	Image_Icon->SetVisibility(ESlateVisibility::Collapsed);

	Text_Name->SetText(FText::GetEmpty());
	Text_Description->SetText(FText::GetEmpty());
}
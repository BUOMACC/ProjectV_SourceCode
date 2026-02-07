// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupWidget.h"
#include "Components/VerticalBox.h"
#include "Inventory/InventoryComponent.h"
#include "PickupEntryWidget.h"

UPickupWidget::UPickupWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	EntryWidgetClass = nullptr;
	EntryCount = 5;
	EntryLifeTime = 3.f;
}

void UPickupWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	VB_Content->ClearChildren();

	if (EntryWidgetClass)
	{
		for (int32 i = 0; i < EntryCount; i++)
		{
			UPickupEntryWidget* EntryWidget = CreateWidget<UPickupEntryWidget>(this, EntryWidgetClass);
			VB_Content->AddChildToVerticalBox(EntryWidget);
		}
	}
}

void UPickupWidget::NativeConstruct()
{
	Super::NativeConstruct();

	EntryWidgets.Empty();

	// Regist Widgets
	for (UWidget* ChildWidget : VB_Content->GetAllChildren())
	{
		if (UPickupEntryWidget* EntryWidget = Cast<UPickupEntryWidget>(ChildWidget))
		{
			EntryWidgets.Add(EntryWidget);
			EntryWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	// Bind Event
	if (APawn* Pawn = GetOwningPlayerPawn())
	{
		if (UInventoryComponent* InventoryComponent = Pawn->GetComponentByClass<UInventoryComponent>())
		{
			InventoryComponent->OnInventoryItemChanged.AddDynamic(this, &ThisClass::OnInventoryItemChanged);
		}
	}
}

void UPickupWidget::PushInfo(UItemInstance* ItemInstance, int32 CountDelta)
{
	if (UPickupEntryWidget* EntryWidget = GetDesiredEntryWidget())
	{
		EntryWidget->UpdateWidget(ItemInstance, CountDelta, EntryLifeTime);
	}
}

UPickupEntryWidget* UPickupWidget::GetDesiredEntryWidget() const
{
	for (UPickupEntryWidget* EntryWidget : EntryWidgets)
	{
		if (EntryWidget->IsEntryEnabled() == false)
		{
			return EntryWidget;
		}
	}
	return nullptr;
}

void UPickupWidget::OnInventoryItemChanged(UItemInstance* ItemInstance, int32 OldCount, int32 NewCount)
{
	int32 CountDelta = FMath::Max(0, NewCount - OldCount);
	PushInfo(ItemInstance, CountDelta);
}

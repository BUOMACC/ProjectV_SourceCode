// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryBagWidget.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/ItemInstance.h"
#include "Components/VerticalBox.h"

#include "UI/Inventory/ItemEntryWidget.h"

UInventoryBagWidget::UInventoryBagWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UInventoryBagWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInventoryBagWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

TArray<UItemEntryWidget*> UInventoryBagWidget::LoadItems(UInventoryComponent* InventoryComponent)
{
	TArray<UItemEntryWidget*> Result;
	Result.Empty();

	ClearEntries();

	if (InventoryComponent)
	{
		const TArray<UItemInstance*> AllItems = InventoryComponent->GetAllItems();
		for (int32 i = 0; i < AllItems.Num(); i++)
		{
			if (AllItems[i] == nullptr)
				continue;

			UItemEntryWidget* EntryWidget = AddEntry(AllItems[i]);
			Result.Add(EntryWidget);
		}

		if (!InventoryComponent->OnInventoryItemChanged.Contains(this, TEXT("OnInventoryItemChanged")))
		{
			InventoryComponent->OnInventoryItemChanged.AddDynamic(this, &UInventoryBagWidget::OnInventoryItemChanged);
		}
	}
	
	return Result;
}

UItemEntryWidget* UInventoryBagWidget::AddEntry(UItemInstance* ItemInstance)
{
	if (ItemInstance == nullptr || ItemInstance->GetCount() <= 0)
		return nullptr;

	UItemEntryWidget* EntryWidget = CreateWidget<UItemEntryWidget>(GetOwningPlayer(), EntryWidgetClass);
	EntryWidget->UpdateWidget(ItemInstance);

	VerticalBox_Items->AddChildToVerticalBox(EntryWidget);
	return EntryWidget;
}

void UInventoryBagWidget::ClearEntries()
{
	VerticalBox_Items->ClearChildren();
}

UItemEntryWidget* UInventoryBagWidget::FindSlot(UItemInstance* ItemInstance) const
{
	for (UWidget* Widget : VerticalBox_Items->GetAllChildren())
	{
		if (UItemEntryWidget* EntryWidget = Cast<UItemEntryWidget>(Widget))
		{
			if (EntryWidget->GetSlotItem() == ItemInstance)
			{
				return EntryWidget;
			}
		}
	}
	return nullptr;
}

void UInventoryBagWidget::OnInventoryItemChanged(UItemInstance* ItemInstance, int32 OldCount, int32 NewCount)
{
	if (UItemEntryWidget* EntryWidget = FindSlot(ItemInstance))
	{
		#pragma region Memo
		/*
		*	ItemInstance의 GetCount()를 사용할때 주의해야 합니다.
		*	멀티 플레이 환경에서, OnInventoryItemChanged 이벤트가 아이템 삭제에 의해 호출된 경우
		*	ItemInstance의 카운트는 복제되기 전 카운트 값을 가지게 되므로 파라미터 NewCount 사용을 권장합니다.
		*/
		#pragma endregion
		if (NewCount <= 0)
		{
			EntryWidget->RemoveFromParent();
			return;
		}
		EntryWidget->UpdateWidget(ItemInstance);
	}
}

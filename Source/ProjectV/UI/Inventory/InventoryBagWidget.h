// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryBagWidget.generated.h"

class UInventoryComponent;
class UInventoryEntryWidget;
class UItemEntryWidget;
class UItemInstance;

class UVerticalBox;

/**
 *	아이템 목록을 표시하는 위젯입니다.
 *	현재 보유하고 있는 아이템을 불러와 구성됩니다.
 */
UCLASS()
class PROJECTV_API UInventoryBagWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UInventoryBagWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	/* 주어진 인벤토리 정보로 아이템 항목들을 생성하고, 생성된 항목들을 반환합니다. */
	TArray<UItemEntryWidget*> LoadItems(UInventoryComponent* InventoryComponent);

	/* 입력받은 아이템 정보로 리스트에 항목을 추가합니다. */
	UItemEntryWidget* AddEntry(UItemInstance* ItemInstance);

	/* 모든 항목을 삭제합니다. */
	void ClearEntries();

	/* 입력받은 아이템을 가진 슬롯을 찾습니다. */
	UItemEntryWidget* FindSlot(UItemInstance* ItemInstance) const;

protected:
	UFUNCTION()
	void OnInventoryItemChanged(UItemInstance* ItemInstance, int32 OldCount, int32 NewCount);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Inventory Bag Widget", meta = (BindWidget))
	TObjectPtr<UVerticalBox> VerticalBox_Items;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory Bag Widget")
	TSubclassOf<UItemEntryWidget> EntryWidgetClass;
};

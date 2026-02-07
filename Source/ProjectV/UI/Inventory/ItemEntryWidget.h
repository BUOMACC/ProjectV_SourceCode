// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemEntryWidget.generated.h"

class UBorder;
class UWidgetSwitcher;
class UItemInstance;
class UItemSlotWidget;
class UTextBlock;

class USoundBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemEntryHighlightStateChanged, bool, bIsHighlighted, UItemInstance*, ItemInstance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemEntryClicked, UItemEntryWidget*, SlotWidget);

/**
 *	인벤토리에 표시되는 아이템 항목 위젯입니다.
 */
UCLASS()
class PROJECTV_API UItemEntryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnItemEntryHighlightStateChanged OnItemEntryHighlightStateChanged;

	UPROPERTY(BlueprintAssignable)
	FOnItemEntryClicked OnItemEntryClicked;

public:
	UItemEntryWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	/* 입력받은 아이템의 정보로 업데이트 합니다. */
	void UpdateWidget(UItemInstance* ItemInstance);

	void SetHighlight(bool bHighlight);

	UItemInstance* GetSlotItem() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Entry Widget")
	TObjectPtr<USoundBase> HighlightSound;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Item Entry Widget", meta = (BindWidget))
	TObjectPtr<UItemSlotWidget> ItemSlotWidget;

	UPROPERTY(BlueprintReadOnly, Category = "Item Entry Widget", meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher_Name;

	UPROPERTY(BlueprintReadOnly, Category = "Item Entry Widget", meta = (BindWidget))
	TObjectPtr<UBorder> Border_Name_Default;

	UPROPERTY(BlueprintReadOnly, Category = "Item Entry Widget", meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Name_Default;

	UPROPERTY(BlueprintReadOnly, Category = "Item Entry Widget", meta = (BindWidget))
	TObjectPtr<UBorder> Border_Name_Highlight;

	UPROPERTY(BlueprintReadOnly, Category = "Item Entry Widget", meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Name_Highlight;

private:
	bool bHighlighted;

	UPROPERTY()
	TObjectPtr<UItemInstance> CachedItemInstance;
};

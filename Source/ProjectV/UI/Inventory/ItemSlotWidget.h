// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlotWidget.generated.h"

class UItemInstance;
class UImage;
class UTextBlock;
class UWidgetSwitcher;
class UOverlay;

/**
 *	아이템 아이콘을 표기하는 슬롯 위젯입니다.
 */
UCLASS()
class PROJECTV_API UItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UItemSlotWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateWidget(UItemInstance* ItemInstance);

protected:
	void SetIcon(UItemInstance* ItemInstance);
	void SetGrade(UItemInstance* ItemInstance);
	void SetCount(UItemInstance* ItemInstance);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Item Slot Widget", meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;

	UPROPERTY(BlueprintReadOnly, Category = "Item Slot Widget", meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Count;

	UPROPERTY(BlueprintReadOnly, Category = "Item Slot Widget", meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher_Tier;
	
	UPROPERTY(BlueprintReadOnly, Category = "Item Slot Widget", meta = (BindWidget))
	TObjectPtr<UOverlay> Overlay_Tier_Normal;

	UPROPERTY(BlueprintReadOnly, Category = "Item Slot Widget", meta = (BindWidget))
	TObjectPtr<UOverlay> Overlay_Tier_Rare;

	UPROPERTY(BlueprintReadOnly, Category = "Item Slot Widget", meta = (BindWidget))
	TObjectPtr<UOverlay> Overlay_Tier_Epic;

	UPROPERTY(BlueprintReadOnly, Category = "Item Slot Widget", meta = (BindWidget))
	TObjectPtr<UOverlay> Overlay_Tier_Legendary;
};

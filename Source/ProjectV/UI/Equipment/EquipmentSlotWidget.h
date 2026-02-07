// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/QuickSlotComponent.h"
#include "EquipmentSlotWidget.generated.h"

class UImage;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquipmentSlotHighlightStateChanged, bool, bIsHighlighted, UItemInstance*, ItemInstance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquipmentSlotClicked, UEquipmentSlotWidget*, InSlotWidget, const FPointerEvent&, InMouseEvent);

/**
 *	장비창에서 표기되는 장착 아이템 슬롯입니다.s
 */
UCLASS()
class PROJECTV_API UEquipmentSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnEquipmentSlotHighlightStateChanged OnEquipmentSlotHighlightStateChanged;

	UPROPERTY(BlueprintAssignable)
	FOnEquipmentSlotClicked OnEquipmentSlotClicked;

public:
	UEquipmentSlotWidget(const FObjectInitializer& ObjectInitializer);

	void Init(EQuickSlotType InSlotType, int32 InSlotIndex);

protected:
	virtual void NativeConstruct() override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	/* 입력받은 아이템 정보로 장착 퀵슬롯 위젯을 업데이트합니다. */
	void UpdateWidget(UItemInstance* ItemInstance, bool bPlaySound = false);

	/* 장착 퀵슬롯 위젯을 비웁니다. */
	void ClearWidget();

	UItemInstance* GetSlotItem() const;

	EQuickSlotType GetSlotType() const;

	int32 GetSlotIndex() const;

protected:
	void SetHighlight(bool bHighlight);
	void SetIcon(UItemInstance* ItemInstance);
	void SetCount(UItemInstance* ItemInstance);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Equipment Slot Widget", meta = (BindWidget))
	TObjectPtr<UImage> Image_Highlight;

	UPROPERTY(BlueprintReadOnly, Category = "Equipment Slot Widget", meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;

	UPROPERTY(BlueprintReadOnly, Category = "Equipment Slot Widget", meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Count;

protected:
	EQuickSlotType SlotType;
	int32 SlotIndex;

	/* 슬롯을 마우스 등으로 호버하여 선택된 상태가 된 경우 호출되는 사운드입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Equipment Slot Widget")
	TObjectPtr<USoundBase> HighlightSound;

	/* 슬롯의 아이템이 장착된 경우 호출되는 사운드입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Equipment Slot Widget")
	TObjectPtr<USoundBase> SlotEquipSound;

	/* 슬롯의 아이템 장착이 해제된 경우 호출되는 사운드입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Equipment Slot Widget")
	TObjectPtr<USoundBase> SlotUnEquipSound;

private:
	bool bHighlighted;

	UPROPERTY()
	TObjectPtr<UItemInstance> CachedItemInstance;
};

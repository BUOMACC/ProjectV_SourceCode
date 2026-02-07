// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/QuickSlotComponent.h"
#include "QuickSlotWidget.generated.h"

class UItemInstance;
class UImage;
class UTextBlock;

/**
 *	인게임 HUD에 표시되는 퀵슬롯 하나를 표현하는 위젯입니다.
 */
UCLASS()
class PROJECTV_API UQuickSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UQuickSlotWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativePreConstruct();
	virtual void NativeOnInitialized();
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable, Category = "QuickSlot HUD Widget")
	void InitWidget(UQuickSlotComponent* QuickSlotComponent);

public:
	/* 입력받은 아이템 정보로 장착 퀵슬롯 위젯을 업데이트합니다. */
	void UpdateWidget();

	/* 장착 퀵슬롯 위젯을 비웁니다. */
	void ClearWidget();

	UItemInstance* GetSlotItem() const;

	EQuickSlotType GetSlotType() const;

	void SetSlotIndex(int32 NewSlotIndex, bool bPlaySound = false, bool bPlayAnimation = false);
	int32 GetSlotIndex() const;

protected:
	void SetIcon(UItemInstance* ItemInstance);
	void SetCount(UItemInstance* ItemInstance);
	void SetName(UItemInstance* ItemInstance);


protected:
	UPROPERTY(BlueprintReadOnly, Category = "QuickSlot Widget", meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;

	UPROPERTY(BlueprintReadOnly, Category = "QuickSlot Widget", meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Count;

	UPROPERTY(BlueprintReadOnly, Category = "QuickSlot Widget", meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Name;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "QuickSlot Widget", meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> SwitchAnimation;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "QuickSlot Widget", meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> NameAnimation;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "QuickSlot Widget")
	EQuickSlotType SlotType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "QuickSlot Widget")
	TObjectPtr<USoundBase> SlotSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "QuickSlot Widget")
	bool bShowCountText;

private:
	UPROPERTY()
	TObjectPtr<UItemInstance> CachedItemInstance;

	UPROPERTY()
	TObjectPtr<UQuickSlotComponent> CachedQuickSlotComponent;

	int32 SlotIndex;
};

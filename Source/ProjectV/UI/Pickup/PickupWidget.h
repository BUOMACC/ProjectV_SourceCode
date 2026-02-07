// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PickupWidget.generated.h"

class UItemInstance;
class UPickupEntryWidget;
class UVerticalBox;

/**
 *	PickupEntryWidget을 그룹 단위로 관리합니다.
 */
UCLASS()
class PROJECTV_API UPickupWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPickupWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

protected:
	/* 알림을 표시합니다. */
	void PushInfo(UItemInstance* ItemInstance, int32 CountDelta);

	/* 사용할 수 있는 Entry Widget을 반환합니다. */
	UPickupEntryWidget* GetDesiredEntryWidget() const;

protected:
	UFUNCTION()
	void OnInventoryItemChanged(UItemInstance* ItemInstance, int32 OldCount, int32 NewCount);

protected:
	/* 표시될 엔트리 위젯 클래스입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup Widget")
	TSubclassOf<UPickupEntryWidget> EntryWidgetClass;

	/* 표시될 위젯의 개수입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup Widget")
	int32 EntryCount;

	/* 엔트리 위젯이 유지될 시간입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup Widget")
	float EntryLifeTime;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Pickup Widget", meta = (BindWidget))
	TObjectPtr<UVerticalBox> VB_Content;

private:
	/* 아래 -> 위 순서로 저장된 Entry 목록입니다. */
	UPROPERTY()
	TArray<TObjectPtr<UPickupEntryWidget>> EntryWidgets;
};

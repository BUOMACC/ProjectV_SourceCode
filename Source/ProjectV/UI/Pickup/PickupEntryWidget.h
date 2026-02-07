// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PickupEntryWidget.generated.h"

class UImage;
class UTextBlock;
class UWidgetAnimation;

class UItemInstance;

/**
 *	아이템을 획득했을때 알림을 표시하는 항목 위젯입니다.
 */
UCLASS()
class PROJECTV_API UPickupEntryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPickupEntryWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	/*
	*	위젯 정보를 업데이트하고 보여줍니다.
	*	 @param ItemInstance 데미지를 가할때 이를 책임지는 실제 공격자입니다.
	*	 @param CountDelta 변화된 개수 차이입니다.
	*	 @param InLifeTime 사라지기까지 유지될 시간입니다.
	*/
	void UpdateWidget(UItemInstance* ItemInstance, int32 CountDelta, float InLifeTime = 5.f);
	void ClearWidget();

	bool IsEntryEnabled() const { return bIsEntryEnabled; }

protected:
	UFUNCTION()
	void OnShowAnimationFinished();

	UFUNCTION()
	void OnHideAnimationFinished();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Pickup Entry Widget", meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;

	UPROPERTY(BlueprintReadOnly, Category = "Pickup Entry Widget", meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Name;

	UPROPERTY(BlueprintReadOnly, Category = "Pickup Entry Widget", meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Count;

protected:
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Pickup Entry Widget", meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> ShowAnimation;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Pickup Entry Widget", meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> HideAnimation;

private:
	FWidgetAnimationDynamicEvent ShowAnimationFinishedEvent;
	FWidgetAnimationDynamicEvent HideAnimationFinishedEvent;
	FTimerHandle TimerHandle;

	float LifeTime;
	bool bIsEntryEnabled;
};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CheckPointEntryWidget.generated.h"

class UWidgetSwitcher;
class UBorder;
class UTextBlock;

DECLARE_MULTICAST_DELEGATE(FOnCheckPointEntryClicked);

/**
 *	체크 포인트 지점에 상호작용시 표시되는 선택지 엔트리입니다.
 */
UCLASS()
class PROJECTV_API UCheckPointEntryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UCheckPointEntryWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	FOnCheckPointEntryClicked OnCheckPointEntryClicked;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CheckPoint Entry Widget")
	FText DescText;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CheckPoint Entry Widget")
	TObjectPtr<USoundBase> HoverSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CheckPoint Entry Widget")
	TObjectPtr<USoundBase> PressedSound;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "CheckPoint Entry Widget", meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher_Desc;

	UPROPERTY(BlueprintReadOnly, Category = "CheckPoint Entry Widget", meta = (BindWidget))
	TObjectPtr<UBorder> Border_Desc_Unhovered;

	UPROPERTY(BlueprintReadOnly, Category = "CheckPoint Entry Widget", meta = (BindWidget))
	TObjectPtr<UBorder> Border_Desc_Hovered;

	UPROPERTY(BlueprintReadOnly, Category = "CheckPoint Entry Widget", meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Desc_Unhovered;

	UPROPERTY(BlueprintReadOnly, Category = "CheckPoint Entry Widget", meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Desc_Hovered;
};

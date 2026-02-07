// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckPointEntryWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"

UCheckPointEntryWidget::UCheckPointEntryWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DescText = FText::FromString(TEXT("Desc Text Here."));
}

void UCheckPointEntryWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	TextBlock_Desc_Hovered->SetText(DescText);
	TextBlock_Desc_Unhovered->SetText(DescText);
}

void UCheckPointEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCheckPointEntryWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	WidgetSwitcher_Desc->SetActiveWidget(Border_Desc_Hovered);

	if (HoverSound)
	{
		PlaySound(HoverSound);
	}
}

void UCheckPointEntryWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	WidgetSwitcher_Desc->SetActiveWidget(Border_Desc_Unhovered);
}

FReply UCheckPointEntryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		if (PressedSound)
		{
			PlaySound(PressedSound);
		}

		OnCheckPointEntryClicked.Broadcast();
		return FReply::Handled();
	}

	return Reply;
}

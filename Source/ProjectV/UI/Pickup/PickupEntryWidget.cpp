// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupEntryWidget.h"
#include "Animation/WidgetAnimation.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Inventory/ItemInstance.h"
#include "Inventory/Fragment/ItemFragment_Inventory.h"

UPickupEntryWidget::UPickupEntryWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TimerHandle.Invalidate();
	ShowAnimationFinishedEvent.Clear();
	HideAnimationFinishedEvent.Clear();

	LifeTime = 0.f;
	bIsEntryEnabled = false;
}

void UPickupEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ShowAnimationFinishedEvent.BindDynamic(this, &ThisClass::OnShowAnimationFinished);
	HideAnimationFinishedEvent.BindDynamic(this, &ThisClass::OnHideAnimationFinished);

	BindToAnimationFinished(ShowAnimation, ShowAnimationFinishedEvent);
	BindToAnimationFinished(HideAnimation, HideAnimationFinishedEvent);
}

void UPickupEntryWidget::UpdateWidget(UItemInstance* ItemInstance, int32 CountDelta, float InLifeTime)
{
	ClearWidget();

	if (!IsValid(ItemInstance) || CountDelta <= 0)
	{
		return;
	}

	if (const UItemFragment_Inventory* InventoryFragment = ItemInstance->FindItemFragmentByClass<UItemFragment_Inventory>())
	{
		Image_Icon->SetBrushFromTexture(InventoryFragment->DisplayIcon);
		Text_Name->SetText(InventoryFragment->DisplayName);
		Text_Count->SetText(FText::AsNumber(CountDelta));
	}

	LifeTime = InLifeTime;
	bIsEntryEnabled = true;

	SetVisibility(ESlateVisibility::HitTestInvisible);
	PlayAnimation(ShowAnimation);
}

void UPickupEntryWidget::ClearWidget()
{
	Text_Name->SetText(FText::GetEmpty());
	Text_Count->SetText(FText::GetEmpty());

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void UPickupEntryWidget::OnShowAnimationFinished()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			PlayAnimation(HideAnimation);
		}, LifeTime, false);
}

void UPickupEntryWidget::OnHideAnimationFinished()
{
	// clean up
	bIsEntryEnabled = false;
	ClearWidget();
	SetVisibility(ESlateVisibility::Hidden);
}

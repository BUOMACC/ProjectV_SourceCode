// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyLifeWidget.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"
#include "Animation/WidgetAnimation.h"

UEnemyLifeWidget::UEnemyLifeWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsLost = false;
}

void UEnemyLifeWidget::InitializeLifeWidget(int32 Size)
{
	SB_Life->SetWidthOverride(Size);
	SB_Life->SetHeightOverride(Size);

	bIsLost = false;
	Image_Life->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UEnemyLifeWidget::SetLifeLostState(bool bIsNewLost, bool bPlayWithAnimation)
{
	if (bIsLost == bIsNewLost)
		return;

	bIsLost = bIsNewLost;

	if (bIsLost)
	{
		if (bPlayWithAnimation)
		{
			PlayAnimation(LifeLostAnimation);
		}
		else
		{
			Image_Life->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		/* Animation에 의해 RenderOpacity가 변경되므로, 보이도록 값을 돌려놓음 */
		SetRenderOpacity(1.f);
		Image_Life->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

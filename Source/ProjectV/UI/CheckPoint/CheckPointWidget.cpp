// Fill out your copyright notice in the Description page of Project Settings.

#include "CheckPointWidget.h"
#include "CheckPointEntryWidget.h"
#include "CombatGameplayTags.h"
#include "Animation/WidgetAnimation.h"
#include "AbilitySystemBlueprintLibrary.h"

UCheckPointWidget::UCheckPointWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FadeOutFinishedEvent.Clear();
}

void UCheckPointWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Entry_Rest->OnCheckPointEntryClicked.AddUObject(this, &ThisClass::OnCheckPointEntryClicked_Rest);
	Entry_Travel->OnCheckPointEntryClicked.AddUObject(this, &ThisClass::OnCheckPointEntryClicked_Travel);
	Entry_Exit->OnCheckPointEntryClicked.AddUObject(this, &ThisClass::OnCheckPointEntryClicked_Exit);

	FadeOutFinishedEvent.BindDynamic(this, &ThisClass::OnFadeOutFinished);
	FadeInFinishedEvent.BindDynamic(this, &ThisClass::OnFadeInFinished);

	BindToAnimationFinished(FadeOutAnim, FadeOutFinishedEvent);
	BindToAnimationFinished(FadeInAnim, FadeInFinishedEvent);
}

void UCheckPointWidget::NativeOnActivated()
{
	if (AcitvateSound)
	{
		PlaySound(AcitvateSound);
	}
}

void UCheckPointWidget::OnCheckPointEntryClicked_Rest()
{
	PlayAnimation(FadeOutAnim);
}

void UCheckPointWidget::OnCheckPointEntryClicked_Travel()
{
	DeactivateWidget();
}

void UCheckPointWidget::OnCheckPointEntryClicked_Exit()
{
	DeactivateWidget();
}

void UCheckPointWidget::OnFadeOutFinished()
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPlayerPawn(), FCombatGameplayTags::Get().Event_Rest, FGameplayEventData());
	PlayAnimation(FadeInAnim);
}

void UCheckPointWidget::OnFadeInFinished()
{
	DeactivateWidget();
}

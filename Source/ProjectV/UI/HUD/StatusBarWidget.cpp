// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusBarWidget.h"
#include "AbilitySystemGlobals.h"
#include "Character/CombatCharacter.h"
#include "GameFramework/Pawn.h"

#include "Components/ProgressBar.h"

UStatusBarWidget::UStatusBarWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AnimatedValue = 0.f;
	CurrentValue = 0.f;
	MaxValue = 0.f;
	bUseAnimation = true;
	AnimationDelay = 0.75f;
	AnimationInterpSpeed = 200.f;

	CurrentValueStatusDelegateHandle.Reset();
	MaxValueStatusDelegateHandle.Reset();
}

void UStatusBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UStatusBarWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (Pawn.IsValid())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Pawn.Get()))
		{
			ASC->GetGameplayAttributeValueChangeDelegate(CurrentValueAttribute).Remove(CurrentValueStatusDelegateHandle);
			CurrentValueStatusDelegateHandle.Reset();

			ASC->GetGameplayAttributeValueChangeDelegate(CurrentValueAttribute).Remove(MaxValueStatusDelegateHandle);
			MaxValueStatusDelegateHandle.Reset();
		}
	}

	GetWorld()->GetTimerManager().ClearTimer(AnimationDelayTimerHandle);
}

void UStatusBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Update Back ProgressBar
	if (bUseAnimation && ProgressBar_Back)
	{
		AnimatedValue = FMath::FInterpConstantTo(AnimatedValue, CurrentValue, InDeltaTime, AnimationInterpSpeed);
		
		float NewPercent = (MaxValue == 0.f) ? 0.f : (AnimatedValue / MaxValue);

		ProgressBar_Back->SetPercent(NewPercent);
	}
}

void UStatusBarWidget::OnCurrentValueStatusChanged(const FOnAttributeChangeData& ChangeData)
{
	float OldValue = ChangeData.OldValue;
	float NewValue = ChangeData.NewValue;

	CurrentValue = NewValue;
	
	UpdateMainProgressBar();

	// Back ProgressBar 보간 애니메이션 중지, {AnimationDelay}초 후 재개
	if (ProgressBar_Back)
	{
		bUseAnimation = false;
		GetWorld()->GetTimerManager().SetTimer(AnimationDelayTimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				bUseAnimation = true;
			}), AnimationDelay, false);
	}
}

void UStatusBarWidget::OnMaxValueStatusChanged(const FOnAttributeChangeData& ChangeData)
{
	float OldValue = ChangeData.OldValue;
	float NewValue = ChangeData.NewValue;

	MaxValue = NewValue;

	UpdateMainProgressBar();
}

void UStatusBarWidget::InitializeStatusBar(APawn* InNewPawn)
{
	if (Pawn == InNewPawn)
		return;

	if (Pawn.IsValid())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Pawn.Get()))
		{
			ASC->GetGameplayAttributeValueChangeDelegate(CurrentValueAttribute).Remove(CurrentValueStatusDelegateHandle);
			CurrentValueStatusDelegateHandle.Reset();

			ASC->GetGameplayAttributeValueChangeDelegate(MaxValueAttribute).Remove(MaxValueStatusDelegateHandle);
			MaxValueStatusDelegateHandle.Reset();
		}
	}

	if (IsValid(InNewPawn))
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(InNewPawn))
		{
			CurrentValueStatusDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(CurrentValueAttribute).AddUObject(this, &ThisClass::OnCurrentValueStatusChanged);
			MaxValueStatusDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(MaxValueAttribute).AddUObject(this, &ThisClass::OnMaxValueStatusChanged);

			// Initialize & Update here
			{
				bool bFound = false;
				CurrentValue = ASC->GetGameplayAttributeValue(CurrentValueAttribute, bFound);
			}
			{
				bool bFound = false;
				MaxValue = ASC->GetGameplayAttributeValue(MaxValueAttribute, bFound);
			}

			AnimatedValue = CurrentValue;
			
			float Percentage = (MaxValue == 0.f) ? 0.f : CurrentValue / MaxValue;

			ProgressBar_Main->SetPercent(Percentage);

			if (ProgressBar_Back)
				ProgressBar_Back->SetPercent(Percentage);

			UpdateMainProgressBar();
		}
	}
}

void UStatusBarWidget::UpdateMainProgressBar()
{
	float Percentage = (MaxValue == 0.f) ? 0.f : CurrentValue / MaxValue;
	ProgressBar_Main->SetPercent(Percentage);
}
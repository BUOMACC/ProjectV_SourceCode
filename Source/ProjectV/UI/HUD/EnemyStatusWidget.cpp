// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyStatusWidget.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "Abilities/AttributeSet/EnemyAttributeSet.h"
#include "Components/HorizontalBox.h"
#include "EnemyLifeWidget.h"
#include "GameFramework/Character.h"
#include "StatusBarWidget.h"
#include "EnemyHUDWidget.h"

UEnemyStatusWidget::UEnemyStatusWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	LifeWidgetSize = 16;
#if WITH_EDITOR
	PreviewLifeWidgetCount = 0;
#endif
}

void UEnemyStatusWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

#if WITH_EDITOR
	if (IsDesignTime())
	{
		HB_Life->ClearChildren();
		if (LifeWidgetClass)
		{
			for (int32 i = 0; i < PreviewLifeWidgetCount; i++)
			{
				UEnemyLifeWidget* LifeWidget = CreateWidget<UEnemyLifeWidget>(this, LifeWidgetClass);
				LifeWidget->InitializeLifeWidget(LifeWidgetSize);
				HB_Life->AddChildToHorizontalBox(LifeWidget);
			}
		}
	}
#endif
}

void UEnemyStatusWidget::InitializeNameplate_Implementation(ACharacter* TargetCharacter)
{
	HB_Life->ClearChildren();
	LifeWidgetList.Empty();

	if (IsValid(TargetCharacter))
	{
		StatusBar_Health->InitializeStatusBar(TargetCharacter);
		StatusBar_Groggy->InitializeStatusBar(TargetCharacter);

		if (LifeWidgetClass)
		{
			if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetCharacter))
			{
				bool bFound = false;
				float LifeValue = ASC->GetGameplayAttributeValue(UEnemyAttributeSet::GetLifeAttribute(), bFound);
				float MaxLifeValue = ASC->GetGameplayAttributeValue(UEnemyAttributeSet::GetMaxLifeAttribute(), bFound);

				for (int32 i = 1; i <= MaxLifeValue; i++)
				{
					UEnemyLifeWidget* LifeWidget = CreateWidget<UEnemyLifeWidget>(this, LifeWidgetClass);
					LifeWidget->InitializeLifeWidget(LifeWidgetSize);

					if (i > LifeValue)
					{
						LifeWidget->SetLifeLostState(true);
					}

					HB_Life->AddChildToHorizontalBox(LifeWidget);
					LifeWidgetList.Add(LifeWidget);
				}

				LifeValueDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(UEnemyAttributeSet::GetLifeAttribute()).AddUObject(this, &ThisClass::OnLifeValueChanged);
			}
		}
	}
}

void UEnemyStatusWidget::DeinitializeNameplate_Implementation()
{
	// ...
}

void UEnemyStatusWidget::OnLifeValueChanged(const FOnAttributeChangeData& ChangeData)
{
	for (int32 i = 0; i < LifeWidgetList.Num(); i++)
	{
		if (i < ChangeData.NewValue)
		{
			LifeWidgetList[i]->SetLifeLostState(false);
		}
		else
		{
			LifeWidgetList[i]->SetLifeLostState(true, true/* bPlayWithAnimation */);
		}
	}
}

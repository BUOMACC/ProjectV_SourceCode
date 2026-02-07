// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHUDWidget.h"
#include "EnemyLifeWidget.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "Abilities/AttributeSet/EnemyAttributeSet.h"
#include "Character/EnemyCharacter.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "StatusBarWidget.h"

UEnemyHUDWidget::UEnemyHUDWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	LifeWidgetSize = 16;
#if WITH_EDITOR
	PreviewLifeWidgetCount = 0;
#endif
}

void UEnemyHUDWidget::InitWidget(AEnemyCharacter* EnemyCharacter)
{
	if (EnemyCharacter)
	{
		TargetEnemy = EnemyCharacter;

		HB_Life->ClearChildren();
		LifeWidgetList.Empty();

		StatusBar_Health->InitializeStatusBar(EnemyCharacter);
		StatusBar_Groggy->InitializeStatusBar(EnemyCharacter);

		Text_EnemyName->SetText(EnemyCharacter->GetEnemyDisplayName());

		if (LifeWidgetClass)
		{
			if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(EnemyCharacter))
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

void UEnemyHUDWidget::NativePreConstruct()
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

void UEnemyHUDWidget::OnLifeValueChanged(const FOnAttributeChangeData& ChangeData)
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

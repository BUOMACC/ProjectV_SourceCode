// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractWidget.h"
#include "Abilities/Ability/InteractionAbility.h"
#include "Components/TextBlock.h"
#include "Interact/PickupActor.h"

UInteractWidget::UInteractWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UInteractWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ScannedDelegateHandle = UInteractionAbility::OnInteractScanned.AddUObject(this, &UInteractWidget::OnScanned);
}

void UInteractWidget::NativeDestruct()
{
	UInteractionAbility::OnInteractScanned.Remove(ScannedDelegateHandle);
	ScannedDelegateHandle.Reset();
	
	Super::NativeDestruct();
}

void UInteractWidget::OnScanned(ACharacter* Instigator, AActor* ScannedActor)
{
	if (Instigator == GetOwningPlayerPawn())
	{
		if (!IsValid(ScannedActor))
		{
			HideWidget();
			return;
		}
		UpdateWidget(ScannedActor);
	}
}

void UInteractWidget::UpdateWidget(AActor* ScannedActor)
{
	if (ScannedActor->Implements<UInteractionInterface>())
	{
		if (TScriptInterface<IInteractionInterface> Interactable = TScriptInterface<IInteractionInterface>(ScannedActor))
		{
			const FInteractionInfo& InteractionInfo = Interactable->GetInteractionInfo();
			Text_Info->SetText(InteractionInfo.Desc);
			PlayAnimation(ShowAnimation);
		}
	}
}

void UInteractWidget::HideWidget()
{
	PlayAnimation(HideAnimation);
}

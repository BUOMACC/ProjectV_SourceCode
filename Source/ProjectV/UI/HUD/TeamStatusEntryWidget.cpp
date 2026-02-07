// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamStatusEntryWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"
#include "StatusBarWidget.h"

UTeamStatusEntryWidget::UTeamStatusEntryWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// ...
}

void UTeamStatusEntryWidget::InitializeEntry(APawn* InTeamPawn)
{
	if (!IsValid(InTeamPawn))
		return;

	if (APlayerState* PlayerState = InTeamPawn->GetPlayerState())
	{
		Text_Name->SetText(FText::FromString(PlayerState->GetPlayerName()));
		StatusBar_Team->InitializeStatusBar(InTeamPawn);

		ObjectKey = FObjectKey(InTeamPawn);
	}
}

bool UTeamStatusEntryWidget::IsSameCharacterObject(APawn* InTeamPawn) const
{
	if (!IsValid(InTeamPawn))
		return false;

	FObjectKey Rhs(InTeamPawn);
	return ObjectKey == Rhs;
}

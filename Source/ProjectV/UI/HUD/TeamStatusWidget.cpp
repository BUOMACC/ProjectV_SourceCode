// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamStatusWidget.h"
#include "TeamStatusEntryWidget.h"
#include "Components/VerticalBox.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"
#include "ProjectVGameState.h"

UTeamStatusWidget::UTeamStatusWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITOR
	PreviewWidgetCount = 0;
#endif
}

void UTeamStatusWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (AProjectVGameState* GameState = GetWorld()->GetGameState<AProjectVGameState>())
	{
		GameState->OnPlayerStateAdded.AddUObject(this, &UTeamStatusWidget::OnPlayerStateAdded);
		GameState->OnPlayerStateRemoved.AddUObject(this, &UTeamStatusWidget::OnPlayerStateRemoved);
	}
}

void UTeamStatusWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

#if WITH_EDITOR
	if (IsDesignTime())
	{
		VB_Entries->ClearChildren();
		if (TeamStatusEntryWidgetClass)
		{
			for (int32 i = 0; i < PreviewWidgetCount; i++)
			{
				UTeamStatusEntryWidget* NewEntryWidget = CreateWidget<UTeamStatusEntryWidget>(this, TeamStatusEntryWidgetClass);
				VB_Entries->AddChildToVerticalBox(NewEntryWidget);
			}
		}
	}
#endif
}

void UTeamStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RebuildTeamEntries();
}

void UTeamStatusWidget::RebuildTeamEntries()
{
	ClearTeamEntires();
	if (AProjectVGameState* GameState = GetWorld()->GetGameState<AProjectVGameState>())
	{
		for (const APlayerState* PlayerState : GameState->PlayerArray)
		{
			AddTeamEntry(PlayerState->GetPawn());
		}
	}
}

void UTeamStatusWidget::AddTeamEntry(APawn* NewTeamPawn)
{
	if (!IsValid(NewTeamPawn) || !TeamStatusEntryWidgetClass)
		return;

	// 내 Pawn과 동일한 경우에는 무시
	if (GetOwningPlayerPawn() == NewTeamPawn)
		return;

	if (!FindTeamEntry(NewTeamPawn))
	{
		UTeamStatusEntryWidget* NewEntryWidget = CreateWidget<UTeamStatusEntryWidget>(this, TeamStatusEntryWidgetClass);
		NewEntryWidget->InitializeEntry(NewTeamPawn);
		VB_Entries->AddChildToVerticalBox(NewEntryWidget);
		Entries.Add(NewEntryWidget);
	}
}

void UTeamStatusWidget::RemoveTeamEntry(APawn* NewTeamPawn)
{
	if (!IsValid(NewTeamPawn))
		return;

	if (UTeamStatusEntryWidget* EntryWidget = FindTeamEntry(NewTeamPawn))
	{
		EntryWidget->RemoveFromParent();
		Entries.Remove(EntryWidget);
	}
}

void UTeamStatusWidget::ClearTeamEntires()
{
	for (UTeamStatusEntryWidget* EntryWidget : Entries)
	{
		EntryWidget->RemoveFromParent();
	}
	Entries.Empty();
}

UTeamStatusEntryWidget* UTeamStatusWidget::FindTeamEntry(APawn* TeamPawn) const
{
	for (UTeamStatusEntryWidget* EntryWidget : Entries)
	{
		if (EntryWidget->IsSameCharacterObject(TeamPawn))
		{
			return EntryWidget;
		}
	}
	return nullptr;
}

void UTeamStatusWidget::OnPlayerStateAdded(APlayerState* PlayerState)
{
	RebuildTeamEntries();
}

void UTeamStatusWidget::OnPlayerStateRemoved(APlayerState* PlayerState)
{
	RebuildTeamEntries();
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TeamStatusWidget.generated.h"

class UTeamStatusEntryWidget;
class UVerticalBox;
class APlayerState;

/**
 *	HUD 내 팀원의 상태를 표시하는 위젯입니다.
 */
UCLASS()
class PROJECTV_API UTeamStatusWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UTeamStatusWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

private:
	void RebuildTeamEntries();
	void AddTeamEntry(APawn* NewTeamPawn);
	void RemoveTeamEntry(APawn* NewTeamPawn);
	void ClearTeamEntires();

	UTeamStatusEntryWidget* FindTeamEntry(APawn* TeamPawn) const;

	void OnPlayerStateAdded(APlayerState* PlayerState);
	void OnPlayerStateRemoved(APlayerState* PlayerState);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Team Status Widget")
	TSubclassOf<UTeamStatusEntryWidget> TeamStatusEntryWidgetClass;

#if WITH_EDITORONLY_DATA
	/* * 에디터 전용 * 디자인 타임에 보여질 위젯 수를 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "Team Status Widget|Editor")
	int32 PreviewWidgetCount;
#endif

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Team Status Widget", meta = (BindWidget))
	TObjectPtr<UVerticalBox> VB_Entries;

private:
	UPROPERTY()
	TArray<UTeamStatusEntryWidget*> Entries;
};

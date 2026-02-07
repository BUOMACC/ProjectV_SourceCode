// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TeamStatusEntryWidget.generated.h"

class UTextBlock;
class UStatusBarWidget;

/**
 *	팀 정보를 나타내는 엔트리 위젯입니다.
 */
UCLASS()
class PROJECTV_API UTeamStatusEntryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UTeamStatusEntryWidget(const FObjectInitializer& ObjectInitializer);

public:
	UFUNCTION(BlueprintCallable, Category = "Team Status Entry Widget")
	void InitializeEntry(APawn* InTeamPawn);

public:
	bool IsSameCharacterObject(APawn* InTeamPawn) const;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Team Status Entry Widget", meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Name;

	UPROPERTY(BlueprintReadOnly, Category = "Team Status Entry Widget", meta = (BindWidget))
	TObjectPtr<UStatusBarWidget> StatusBar_Team;

private:
	FObjectKey ObjectKey;
};

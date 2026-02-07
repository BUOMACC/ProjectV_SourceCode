// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ListView.h"
#include "SettingListViewWidget.generated.h"

class UUserSettingBase;
class USettingEntryWidgetBase;

/**
 *	Setting Entry를 표시하는 ListView 위젯입니다.
 */
UCLASS()
class PROJECTV_API USettingListViewWidget : public UListView
{
	GENERATED_BODY()
	
protected:
	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TMap<TSubclassOf<UUserSettingBase>, TSubclassOf<USettingEntryWidgetBase>> EntryWidgetClassMap;
};

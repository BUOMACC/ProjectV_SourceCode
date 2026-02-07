// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingListViewWidget.h"
#include "Setting/UserSettingBase.h"
#include "Setting/Widgets/SettingEntryWidgetBase.h"

UUserWidget& USettingListViewWidget::OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable)
{
	TSubclassOf<USettingEntryWidgetBase> Desired = TSubclassOf<USettingEntryWidgetBase>(DesiredEntryClass);

	if (const UUserSettingBase* Setting = Cast<UUserSettingBase>(Item))
	{
		if (TSubclassOf<USettingEntryWidgetBase>* EntryClass = EntryWidgetClassMap.Find(Setting->GetClass()))
		{
			Desired = *EntryClass;
		}
	}

	USettingEntryWidgetBase& EntryWidget = GenerateTypedEntry<USettingEntryWidgetBase>(Desired, OwnerTable);

	return EntryWidget;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingEntryWidgetBase.h"
#include "Setting/UserSettingBase.h"

void USettingEntryWidgetBase::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	if (UUserSettingBase* SettingObject = Cast<UUserSettingBase>(ListItemObject))
	{
		SetSetting(SettingObject);
	}
}

void USettingEntryWidgetBase::SetSetting(UUserSettingBase* InSetting)
{
	if (Setting != InSetting)
	{
		Setting = InSetting;
		HandleSettingChanged();
	}
}

void USettingEntryWidgetBase::HandleSettingChanged()
{
	// ...
}
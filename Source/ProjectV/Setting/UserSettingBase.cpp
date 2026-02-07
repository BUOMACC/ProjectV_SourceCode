// Fill out your copyright notice in the Description page of Project Settings.


#include "UserSettingBase.h"

UUserSettingBase::UUserSettingBase()
{
	DisplayName = FText::GetEmpty();
}

void UUserSettingBase::Initialize()
{
	// ...
}

void UUserSettingBase::SetDisplayName(const FText& InDisplayName)
{
	DisplayName = InDisplayName;
}

FText UUserSettingBase::GetDisplayName() const
{
	return DisplayName;
}
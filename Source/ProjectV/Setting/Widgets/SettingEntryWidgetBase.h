// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "SettingEntryWidgetBase.generated.h"

class UUserSettingBase;

/**
 *	게임 세팅에 표시되는 항목 하나에 대한 위젯 베이스입니다.
 */
UCLASS()
class PROJECTV_API USettingEntryWidgetBase : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	virtual void ConfirmSettingValue() PURE_VIRTUAL(, );

protected:
	void SetSetting(UUserSettingBase* InSetting);

	virtual void HandleSettingChanged();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Setting Entry Widget")
	TObjectPtr<UUserSettingBase> Setting;
};

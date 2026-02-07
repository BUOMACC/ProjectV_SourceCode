// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UserSettingBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSettingValueChangedEvent);

/**
 *	설정값을 관리하기 위한 베이스 오브젝트입니다.
 *	필요한 인터페이스를 제공합니다.
 */
UCLASS(Abstract)
class PROJECTV_API UUserSettingBase : public UObject
{
	GENERATED_BODY()
	
public:
	UUserSettingBase();

public:
	virtual void Initialize();

	/* 값을 확정한 경우 호출됩니다. */
	virtual void ConfirmValue() PURE_VIRTUAL(, );

public:
	void SetDisplayName(const FText& InDisplayName);
	FText GetDisplayName() const;

protected:
	FText DisplayName;

public:
	UPROPERTY(BlueprintAssignable, Category = "User Setting")
	FOnSettingValueChangedEvent OnSettingValueChangedEvent;
};

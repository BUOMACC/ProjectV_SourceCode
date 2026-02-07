// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CombatDeveloperSettings.generated.h"

UCLASS(config = EditorPerProjectUserSettings, MinimalAPI)
class UCombatDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UCombatDeveloperSettings();

	//~UDeveloperSettings interface
	virtual FName GetCategoryName() const override;
	//~End of UDeveloperSettings interface

public:
	/* 이 값이 true인 경우, 적이 스포너에 의해 스폰되지 않습니다. */
	UPROPERTY(EditAnywhere, config, Category = "Settings")
	bool bDisableEnemySpawn;
};

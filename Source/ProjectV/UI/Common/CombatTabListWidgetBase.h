// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonTabListWidgetBase.h"
#include "CombatTabListWidgetBase.generated.h"

UCLASS()
class PROJECTV_API UCombatTabListWidgetBase : public UCommonTabListWidgetBase
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

	virtual void HandleTabCreation_Implementation(FName TabNameID, UCommonButtonBase* TabButton) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Combat Tab List")
	bool RegisterTabWithText(FName TabNameID, TSubclassOf<UCommonButtonBase> ButtonWidgetType, FText TabButtonText, UWidget* ContentWidget, const int32 TabIndex = -1 /*INDEX_NONE*/);

private:
	TMap<FName, FText> ButtonTextMap;
};

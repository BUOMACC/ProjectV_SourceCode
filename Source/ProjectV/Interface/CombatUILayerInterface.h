// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "CombatUILayerInterface.generated.h"

class UCommonActivatableWidget;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatUILayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *	
 */
class PROJECTV_API ICombatUILayerInterface
{
	GENERATED_BODY()

public:
	/* 해당하는 위젯을 레이어에 추가합니다. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Common UI Interface")
	UCommonActivatableWidget* AddWidget(UPARAM(meta = (Categories = "UI.Layer"))FGameplayTag InLayerTag, TSubclassOf<UCommonActivatableWidget> WidgetToAdd);

	/* 해당하는 레이어의 모든위젯을 닫습니다. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Common UI Interface")
	void ClearWidgets(UPARAM(meta = (Categories = "UI.Layer"))FGameplayTag InLayerTag);

	/* 해당하는 위젯을 닫습니다. 닫으려는 위젯보다 나중에 추가된 위젯이 있다면 함께 닫힙니다. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Common UI Interface")
	void RemoveWidget(UCommonActivatableWidget* InWidgetToRemove);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "CombatUILayerWidget.generated.h"

class UCommonActivatableWidget;
class UCommonActivatableWidgetStack;

USTRUCT(BlueprintType)
struct PROJECTV_API FRegisteredWidgetLayerInfo
{
	GENERATED_BODY()

public:
	FRegisteredWidgetLayerInfo()
		: LayerTag(FGameplayTag::EmptyTag)
		, WidgetStack(nullptr)
	{

	}

public:
	FGameplayTag LayerTag;
	TWeakObjectPtr<UCommonActivatableWidgetStack> WidgetStack;
};

/**
 *	전체적인 UI 계층을 관리하는 레이어 위젯입니다.
 *	모든 게임 위젯들은 해당 레이어에 추가되어 관리됩니다.
 * 
 *	이 위젯을 상속받은 뒤 원하는 레이어를 구성하면 됩니다.
 * 
 *	<등록 방법>
 *	 1) 위젯 내부 OnInitialize 함수에서 RegistWidgetStack() 함수를 호출하여 계층을 등록하세요.
 *	 2) 위젯을 CombatHUD의 DefaultLayerClass로 등록하세요.
 * 
 *	※ 등록을 위한 GameplayTag는 반드시 "UI.Layer" 하위에 있어야 합니다.
 */
UCLASS(Abstract)
class PROJECTV_API UCombatUILayerWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
/*
	######################################
	#			  Functions				 #
	######################################
*/
protected:
	UFUNCTION(BlueprintCallable, Category = "Combat UI|Layer")
	void RegistWidgetStack(UPARAM(meta = (Categories = "UI.Layer"))FGameplayTag InLayerTag, UCommonActivatableWidgetStack* InWidgetStack);

public:
	UCommonActivatableWidget* AddWidget(FGameplayTag& InLayerTag, TSubclassOf<UCommonActivatableWidget> InWidgetToAdd);
	void ClearWidgets(FGameplayTag& InLayerTag);
	void RemoveWidget(UCommonActivatableWidget* InWidgetToRemove);

private:
	UCommonActivatableWidgetStack* FindWidgetStack(FGameplayTag& InLayerTag);

/*
	######################################
	#			  Variables				 #
	######################################
*/
private:
	TArray<FRegisteredWidgetLayerInfo> RegisteredInfo;
};

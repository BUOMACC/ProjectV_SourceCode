// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Interface/CombatUILayerInterface.h"
#include "CombatHUD.generated.h"

class UCommonActivatableWidget;
class UCombatUILayerWidget;

/*
 *	위젯 관리 매니저 용도로 사용
 */
UCLASS()
class PROJECTV_API ACombatHUD : public AHUD, public ICombatUILayerInterface
{
	GENERATED_BODY()
		
/*
	######################################
	#			  Functions				 #
	######################################
*/
public:
	ACombatHUD();

protected:
	virtual void PreInitializeComponents() override;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// ICombatUILayerInterface
	virtual UCommonActivatableWidget* AddWidget_Implementation(UPARAM(meta = (Categories = "UI.Layer"))FGameplayTag InLayerTag, TSubclassOf<UCommonActivatableWidget> WidgetToAdd) override;
	virtual void ClearWidgets_Implementation(UPARAM(meta = (Categories = "UI.Layer"))FGameplayTag InLayerTag) override;
	virtual void RemoveWidget_Implementation(UCommonActivatableWidget* InWidgetToRemove) override;
	// ~ICombatUILayerInterface


/*
	######################################
	#			  Variables				 #
	######################################
*/
protected:
	/* 게임에 기본적으로 사용할 위젯의 레이어 정의(규칙)입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "Combat HUD", meta = (BlueprintProtected))
	TSubclassOf<UCombatUILayerWidget> DefaultLayerWidget;

	UPROPERTY();
	TObjectPtr<UCombatUILayerWidget> LayerWidgetInstance;
};

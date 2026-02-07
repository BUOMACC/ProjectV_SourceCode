// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CombatActivatableWidget.h"
#include "GameHUDWidget.generated.h"

class UInputAction;

/**
 *	인게임 메인 HUD 위젯입니다.
 */
UCLASS()
class PROJECTV_API UGameHUDWidget : public UCombatActivatableWidget
{
	GENERATED_BODY()
	
public:
	UGameHUDWidget(const FObjectInitializer& ObjectInitializer);

	void NativeOnInitialized() override;

protected:
	void HandlePressBack();

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> BackAction;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCommonActivatableWidget> BackWidget;
};

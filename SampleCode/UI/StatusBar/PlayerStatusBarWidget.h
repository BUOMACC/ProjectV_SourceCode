// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatusBarWidget.h"
#include "PlayerStatusBarWidget.generated.h"

/**
 *	플레이어의 상태바를 표시하는 위젯입니다.
 */
UCLASS()
class PROJECTV_API UPlayerStatusBarWidget : public UStatusBarWidget
{
	GENERATED_BODY()
	
public:
	UPlayerStatusBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	UFUNCTION()
	void OnPawnChanged(APawn* OldPawn, APawn* NewPawn);
};

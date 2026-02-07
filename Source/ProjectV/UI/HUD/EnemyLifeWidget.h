// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyLifeWidget.generated.h"

class USizeBox;
class UImage;
class UWidgetAnimation;

UCLASS()
class PROJECTV_API UEnemyLifeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UEnemyLifeWidget(const FObjectInitializer& ObjectInitializer);

public:
	/* 입력받은 크기로 위젯을 초기화합니다. */
	void InitializeLifeWidget(int32 Size);
	
	/* 목숨을 잃었을 때 상태로 변경합니다. */
	void SetLifeLostState(bool bNewIsLost, bool bPlayWithAnimation = false);

	bool IsLost() const { return bIsLost; }

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Enemy Life Widget", meta = (BindWidget))
	TObjectPtr<USizeBox> SB_Life;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy Life Widget", meta = (BindWidget))
	TObjectPtr<UImage> Image_Life;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Enemy Life Widget", meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> LifeLostAnimation;

private:
	bool bIsLost = false;
};

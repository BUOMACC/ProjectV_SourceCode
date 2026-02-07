// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CombatActivatableWidget.h"
#include "CheckPointWidget.generated.h"

class UCheckPointEntryWidget;

/**
 *	체크 포인트 지점에 상호작용시 표시되는 위젯입니다.
 */
UCLASS()
class PROJECTV_API UCheckPointWidget : public UCombatActivatableWidget
{
	GENERATED_BODY()
	
public:
	UCheckPointWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnActivated() override;

protected:
	void OnCheckPointEntryClicked_Rest();
	void OnCheckPointEntryClicked_Travel();
	void OnCheckPointEntryClicked_Exit();

protected:
	UFUNCTION()
	void OnFadeOutFinished();

	UFUNCTION()
	void OnFadeInFinished();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CheckPoint Widget")
	TObjectPtr<USoundBase> AcitvateSound;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "CheckPoint Widget", meta = (BindWidget))
	TObjectPtr<UCheckPointEntryWidget> Entry_Rest;

	UPROPERTY(BlueprintReadOnly, Category = "CheckPoint Widget", meta = (BindWidget))
	TObjectPtr<UCheckPointEntryWidget> Entry_Travel;

	UPROPERTY(BlueprintReadOnly, Category = "CheckPoint Widget", meta = (BindWidget))
	TObjectPtr<UCheckPointEntryWidget> Entry_Exit;

protected:
	UPROPERTY(BlueprintReadOnly, Transient, Category = "CheckPoint Widget", meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeOutAnim;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "CheckPoint Widget", meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeInAnim;

private:
	FWidgetAnimationDynamicEvent FadeOutFinishedEvent;
	FWidgetAnimationDynamicEvent FadeInFinishedEvent;
};

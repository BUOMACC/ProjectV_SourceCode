// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractWidget.generated.h"

class UTextBlock;

/**
 *	HUD에서 상호작용이 가능한 대상을 표시하는 위젯입니다.
 */
UCLASS()
class PROJECTV_API UInteractWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UInteractWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void OnScanned(ACharacter* Instigator, AActor* ScannedActor);

public:
	void UpdateWidget(AActor* ScannedActor);
	void HideWidget();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Interact Widget", meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Info;

protected:
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Interact Widget", meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> ShowAnimation;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Interact Widget", meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> HideAnimation;

private:
	FDelegateHandle ScannedDelegateHandle;
};

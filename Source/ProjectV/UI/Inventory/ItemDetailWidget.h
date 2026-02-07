// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemDetailWidget.generated.h"

class UImage;
class UTextBlock;
class UItemInstance;

/**
 *	아이템의 상세 정보(설명, 능력치 등..)를 표시하는 위젯입니다.
 */
UCLASS()
class PROJECTV_API UItemDetailWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UItemDetailWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

public:
	void UpdateWidget(UItemInstance* ItemInstance);
	void ClearDetail();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Item Detail Widget", meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;

	UPROPERTY(BlueprintReadOnly, Category = "Item Detail Widget", meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Name;

	UPROPERTY(BlueprintReadOnly, Category = "Item Detail Widget", meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Description;
};

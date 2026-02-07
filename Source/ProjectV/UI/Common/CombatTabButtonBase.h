// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "CombatTabButtonBase.generated.h"

class UCommonTextBlock;

/**
 *	CombatTabListWidget과 함께 사용되는 버튼입니다.
 *	탭 버튼의 텍스트를 지정할 수 있는 기능이 포함되어 있습니다.
 */
UCLASS()
class PROJECTV_API UCombatTabButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()
	
public:
	void SetButtonText(FText& InText);

protected:
	virtual void NativeOnCurrentTextStyleChanged() override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Combat Tab Button Base", meta = (BindWidgetOptional))
	TObjectPtr<UCommonTextBlock> Text_Name;
};

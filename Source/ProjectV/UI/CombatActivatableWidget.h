// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CombatActivatableWidget.generated.h"

UENUM(BlueprintType)
enum class ECombatWidgetInputMode : uint8
{
	Default,
	GameAndUI,
	GameOnly,
	UIOnly
};

/**
 *	해당 프로젝트에서 모든 활성화 위젯들은 이 클래스를 상속받아야 합니다.
 *	CommonActivatableWidget대신 사용해야 하며 CombatWidgetInputMode로 위젯이 활성화되었을때 입력 모드를 설정할 수 있습니다.
 */
UCLASS()
class PROJECTV_API UCombatActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
public:
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	ECombatWidgetInputMode CombatWidgetInputMode = ECombatWidgetInputMode::Default;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	EMouseCaptureMode GameMouseCaptureMode = EMouseCaptureMode::CapturePermanently;
};

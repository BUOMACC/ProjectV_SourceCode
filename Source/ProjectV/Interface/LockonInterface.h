// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LockonInterface.generated.h"

class UWidgetComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULockonInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *	Lockon을 허용할 대상에게 구현해 사용
 */
class PROJECTV_API ILockonInterface
{
	GENERATED_BODY()

public:
	/* Lockon시 카메라가 바라볼 위치를 반환 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UWidgetComponent* GetLockonWidget();
};

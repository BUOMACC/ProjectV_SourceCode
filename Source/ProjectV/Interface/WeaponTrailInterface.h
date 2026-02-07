// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponTrailInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWeaponTrailInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *	AnimNotifyState_NiagaraTrail과 함께 사용하며 트레일의 시작 / 끝을 요청하는 인터페이스
 */
class PROJECTV_API IWeaponTrailInterface
{
	GENERATED_BODY()

public:
	/* 트레일을 표시할 시작 / 끝 위치를 반환합니다. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void GetTrailPoint(FVector& OutTrailStart, FVector& OutTrailEnd);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DeathBlowInfo.generated.h"

class UDeathBlowExecutorAbility;

USTRUCT(BlueprintType)
struct PROJECTV_API FDeathBlowInfo
{
	GENERATED_BODY()

public:
	FDeathBlowInfo();

public:
	/* 일반적인 상황에서 치명적인 일격(인살)시 실행되는 실행자의 어빌리티입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UDeathBlowExecutorAbility> DeathBlowAbility_Default;

	/* 배후에서 치명적인 일격(인살)시 실행되는 실행자의 어빌리티입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UDeathBlowExecutorAbility> DeathBlowAbility_Back;
};
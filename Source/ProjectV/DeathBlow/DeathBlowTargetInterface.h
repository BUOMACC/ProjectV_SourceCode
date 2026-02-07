// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DeathBlowInfo.h"
#include "DeathBlowExecutorAbility.h"
#include "DeathBlowTargetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDeathBlowTargetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *	치명적인 일격(인살)이 가능한 대상에게 구현하는 인터페이스
 */
class PROJECTV_API IDeathBlowTargetInterface
{
	GENERATED_BODY()

public:
	virtual FDeathBlowInfo GetDeathBlowInfo() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TSubclassOf<UDeathBlowExecutorAbility> GetDeathBlowExecutorAbility(AActor* Executor);
	virtual TSubclassOf<UDeathBlowExecutorAbility> GetDeathBlowExecutorAbility_Implementation(AActor* Executor);
};

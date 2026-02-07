// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathBlowTargetInterface.h"

// Add default functionality here for any IDeathBlowTargetInterface functions that are not pure virtual.

FDeathBlowInfo IDeathBlowTargetInterface::GetDeathBlowInfo() const
{
    return FDeathBlowInfo();
}

TSubclassOf<UDeathBlowExecutorAbility> IDeathBlowTargetInterface::GetDeathBlowExecutorAbility_Implementation(AActor* Executor)
{
    return nullptr;
}

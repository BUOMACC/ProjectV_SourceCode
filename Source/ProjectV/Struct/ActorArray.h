// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorArray.generated.h"

USTRUCT(BlueprintType)
struct PROJECTV_API FActorArray
{
	GENERATED_BODY()

public:
	FActorArray();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> Actors;
};
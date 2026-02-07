// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ProjectVGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTV_API UProjectVGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
private:
	void Init() override;
};

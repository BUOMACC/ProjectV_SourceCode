// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BranchAction_Base.h"
#include "GameplayTagContainer.h"
#include "BranchAction_SendGameplayEvent.generated.h"

/**
 *	Branch 조건 만족시 이벤트 전달
 */
UCLASS(EditInlineNew, Blueprintable, BlueprintType, DisplayName = "Send Gameplay Event")
class PROJECTV_API UBranchAction_SendGameplayEvent : public UBranchAction_Base
{
	GENERATED_BODY()
	
public:
	UBranchAction_SendGameplayEvent();
	virtual void Activate(ACombatCharacter* Instigator) override;

public:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FGameplayTag GameEventTag;
};

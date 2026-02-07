// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BranchAction_Base.h"
#include "GameplayTagContainer.h"
#include "BranchAction_CancelAbility.generated.h"

/**
 *	Branch 조건 만족시 특정 타입의 어빌리티를 취소
 */
UCLASS(EditInlineNew, Blueprintable, BlueprintType, DisplayName = "Cancel Ability")
class PROJECTV_API UBranchAction_CancelAbility : public UBranchAction_Base
{
	GENERATED_BODY()
	
public:
	UBranchAction_CancelAbility();
	virtual void Activate(ACombatCharacter* Instigator) override;

public:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer CancelAbilityTags;
};

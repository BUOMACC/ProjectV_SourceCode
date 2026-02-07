// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BranchAction_Base.h"
#include "BranchAction_ActivateAbility.generated.h"

class UCombatGameplayAbility;
class UProjectVGameplayAbility;

/**
 *	Branch 조건 만족시 어빌리티를 실행
 */
UCLASS(EditInlineNew, Blueprintable, BlueprintType, DisplayName = "ActivateAbility")
class PROJECTV_API UBranchAction_ActivateAbility : public UBranchAction_Base
{
	GENERATED_BODY()
	
public:
	UBranchAction_ActivateAbility();
	virtual void Activate(ACombatCharacter* Instigator) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UProjectVGameplayAbility> AbilityClass;
};

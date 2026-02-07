// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemFragment_QuickSlot.h"
#include "GameplayTagContainer.h"
#include "ItemFragment_Skill.generated.h"

class UGameplayEffect;
class UGrantAbilityData;

/**
 *	전투기술로 사용할 수 있도록 정보를 추가합니다.
 */
UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class PROJECTV_API UItemFragment_Skill : public UItemFragment_QuickSlot
{
	GENERATED_BODY()
	
public:
	UItemFragment_Skill();

public:
	/* 스킬로 부여할 어빌리티입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quick Slot|Skill")
	TObjectPtr<const UGrantAbilityData> AbilityToGrant;
};

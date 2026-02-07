// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemFragment_QuickSlot.h"
#include "GameplayTagContainer.h"
#include "ItemFragment_Useable.generated.h"

class UGameplayEffect;
class UGrantAbilityData;

/**
 *	아이템을 사용할 수 있도록 정보를 추가합니다.
 */
UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class PROJECTV_API UItemFragment_Useable : public UItemFragment_QuickSlot
{
	GENERATED_BODY()
	
public:
	UItemFragment_Useable();

public:
	/* 사용후 효과로 부여될 어빌리티입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quick Slot|Useable Item")
	TSubclassOf<UGameplayEffect> ItemEffectClass;

	/* 사용될 어빌리티입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quick Slot|Useable Item")
	TObjectPtr<const UGrantAbilityData> AbilityToGrant;

	/* 아이템을 사용할때 보여질 비주얼 메시입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quick Slot|Useable Item")
	TObjectPtr<UStaticMesh> VisualMesh;
};

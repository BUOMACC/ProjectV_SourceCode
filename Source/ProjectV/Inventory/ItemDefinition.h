// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Fragment/ItemFragmentBase.h"
#include "ItemDefinition.generated.h"

/**
 *	아이템 객체에 대한 정의
 *	최소한의 아이템 정보 및 각 상황에 따른 Action, Data가 포함됨
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class PROJECTV_API UItemDefinition : public UObject
{
	GENERATED_BODY()
	
public:
	UItemDefinition();

public:
	UFUNCTION(BlueprintPure, Category = "Item Definition Helpers")
	static UItemDefinition* GetItemDefCDO(TSubclassOf<UItemDefinition> ItemDefinitionClass);

	UFUNCTION(BlueprintPure, Category = "Item Definition Helpers")
	const UItemFragmentBase* FindItemFragmentByClass(TSubclassOf<UItemFragmentBase> ItemFragmentClass) const;

	template <typename ResultClass>
	const ResultClass* FindItemFragmentByClass() const
	{
		return (ResultClass*)FindItemFragmentByClass(ResultClass::StaticClass());
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item", Instanced)
	TArray<TObjectPtr<UItemFragmentBase>> ItemFragments;
};

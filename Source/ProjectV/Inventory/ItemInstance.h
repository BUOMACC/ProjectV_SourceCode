// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemInstance.generated.h"

class UItemDefinition;

/**
 *	Item Definition에 의해 인스턴스화된 아이템을 관리하는 오브젝트입니다.
 *	아이템에 대해 변화하는 동적인 데이터를 관리하거나, 특정 아이템에 대한 행동을 처리하는데 용이합니다.
 *	인스턴스는 Item Definition에 접근할 수 있습니다.
 */
UCLASS(BlueprintType)
class PROJECTV_API UItemInstance : public UObject
{
	GENERATED_BODY()
	
public:
	UItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UObject interface
	virtual bool IsSupportedForNetworking() const override { return true; }
	//~End of UObject interface

	UFUNCTION(BlueprintCallable, Category = "Item Instance")
	void SetItemDef(TSubclassOf<UItemDefinition> InDef);

	UFUNCTION(BlueprintPure, Category = "Item Instance")
	TSubclassOf<UItemDefinition> GetItemDef() const;

	UFUNCTION(BlueprintCallable, Category = "Item Instance")
	void AddCount(int32 CountToAdd);

	UFUNCTION(BlueprintCallable, Category = "Item Instance")
	void SetCount(int32 NewCount);

	UFUNCTION(BlueprintPure, Category = "Item Instance")
	int32 GetCount() const;

	UFUNCTION(BlueprintPure, Category = "Item Instance")
	int32 GetLastCount() const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Item Instance", meta = (DeterminesOutputType = ItemFragmentClass))
	const UItemFragmentBase* FindItemFragmentByClass(TSubclassOf<UItemFragmentBase> ItemFragmentClass) const;

	template <typename ResultClass>
	const ResultClass* FindItemFragmentByClass() const
	{
		return (ResultClass*)FindItemFragmentByClass(ResultClass::StaticClass());
	}

private:
	UPROPERTY(Replicated)
	TSubclassOf<UItemDefinition> ItemDef;

	UPROPERTY(Replicated)
	int32 Count;

	UPROPERTY(Replicated)
	int32 LastCount;
};

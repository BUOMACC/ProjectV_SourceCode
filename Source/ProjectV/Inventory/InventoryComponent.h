// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "InventoryComponent.generated.h"

class UItemDefinition;
class UItemInstance;

/*
*	Item Instance Handler
*	- 슬롯 개념으로 사용되고, 생성된 아이템 인스턴스와 스택 개수를 관리함
*/
USTRUCT(BlueprintType)
struct PROJECTV_API FInventoryItemEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

public:
	FInventoryItemEntry();

public:
	UPROPERTY()
	TObjectPtr<UItemInstance> ItemInstance;
};

/*
*	Item Entry Handling Container
*	- 슬롯단위(Entry)의 아이템(Instance)들을 관리하는 역할
*/
USTRUCT(BlueprintType)
struct PROJECTV_API FInventoryItemList : public FFastArraySerializer
{
	GENERATED_BODY()

public:
	FInventoryItemList();
	FInventoryItemList(UInventoryComponent* InOwnerComponent);

public:
	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FInventoryItemEntry, FInventoryItemList>(Entries, DeltaParms, *this);
	}

	UItemInstance* AddEntry(TSubclassOf<UItemDefinition> ItemDef, int32 ItemCount);
	void RemoveEntry(UItemInstance* Instance);

	TArray<UItemInstance*> GetAllItems() const;

private:
	UPROPERTY()
	TArray<FInventoryItemEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UInventoryComponent> OwnerComponent;

private:
	friend UInventoryComponent;
};

template<>
struct TStructOpsTypeTraits<FInventoryItemList> : public TStructOpsTypeTraitsBase2<FInventoryItemList>
{
	enum { WithNetDeltaSerializer = true };
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnInventoryItemChanged, UItemInstance*, ItemInstance, int32, OldCount, int32, NewCount);

/*
*	Item Handling Component
*	- 빈 공간(슬롯)은 없이 순서대로 아이템을 할당하는 방식으로 동작함
*/
UCLASS(BlueprintType, ClassGroup = (Inventory), meta = (BlueprintSpawnableComponent))
class PROJECTV_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

/*
*	#################################
*	#			Events				#
*	#################################
*/
public:
	UPROPERTY(BlueprintAssignable)
	FOnInventoryItemChanged OnInventoryItemChanged;

/*
*	#################################
*	#			Functions			#
*	#################################
*/
public:	
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void ReadyForReplication() override;

public:
	/* 인벤토리에 아이템을 추가합니다. 가장 마지막에 추가된 아이템의 인스턴스가 반환됩니다. */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	UItemInstance* AddItemDefinition(TSubclassOf<UItemDefinition> ItemDef, int32 AddCount = 1);

	/* 아이템 개수를 증가시킵니다. */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	void AddItemCount(UItemInstance* ItemInstance, int32 AddCount = 1);

	/* 인벤토리에서 아이템을 제거합니다. */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	void RemoveItemDefinition(TSubclassOf<UItemDefinition> ItemDef, int32 RemoveCount = 1);

	/* 아이템 개수를 감소시킵니다. */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	void RemoveItemCount(UItemInstance* ItemInstance, int32 RemoveCount = 1);

	/* 모든 아이템 인스턴스를 가져옵니다. */
	UFUNCTION(BlueprintPure, Category = "Inventory")
	TArray<UItemInstance*> GetAllItems() const;

	/* 해당하는 아이템의 개수를 가져옵니다. */
	UFUNCTION(BlueprintPure, Category = "Inventory")
	int32 GetItemCount(TSubclassOf<UItemDefinition> ItemDef) const;

private:
	bool OwnerHasAuthority() const;

public:
	void BroadcastItemChangeMessage(const FInventoryItemEntry& Entry, int32 OldCount, int32 NewCount);

/*
*	#################################
*	#			Variables			#
*	#################################
*/
private:
	UPROPERTY(Replicated)
	FInventoryItemList InventoryList;
};

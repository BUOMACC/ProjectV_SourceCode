// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Data/GrantAbilityData.h"
#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "EquipmentComponent.generated.h"

class UActorComponent;
class UCombatAbilitySystemComponent;
class UEquipmentDefinition;
class UEquipmentInstance;
class UEquipmentComponent;
class UObject;
struct FFrame;
struct FEquipmentList;
struct FNetDeltaSerializeInfo;
struct FReplicationFlags;

#pragma region Memo
/* Lyra Based Equipment Component */
#pragma endregion

/* 장착한 아이템 하나의 대한 정보를 담고있습니다. */
USTRUCT(BlueprintType)
struct PROJECTV_API FAppliedEquipmentEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

public:
	FAppliedEquipmentEntry() { }

private:
	friend FEquipmentList;
	friend UEquipmentComponent;

	UPROPERTY()
	TSubclassOf<UEquipmentDefinition> EquipmentDefinition;

	UPROPERTY()
	TObjectPtr<UEquipmentInstance> Instance = nullptr;

	// 서버 전용 - 부여된 어빌리티들에 대한 핸들입니다.
	UPROPERTY(NotReplicated)
	FGameplayTag GrantAbilityDataTypeTag;
};

/* 장착한 아이템들을 관리하는 컨테이너입니다. */
USTRUCT(BlueprintType)
struct PROJECTV_API FEquipmentList : public FFastArraySerializer
{
	GENERATED_BODY()

public:
	FEquipmentList() : OwnerComponent(nullptr) { }
	FEquipmentList(UActorComponent* InOwnerComponent) : OwnerComponent(InOwnerComponent) { }

public:
	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FAppliedEquipmentEntry, FEquipmentList>(Entries, DeltaParms, *this);
	}

	UEquipmentInstance* AddEntry(TSubclassOf<UEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(UEquipmentInstance* Instance);

private:
	UCombatAbilitySystemComponent* GetAbilitySystemComponent() const;

	friend UEquipmentComponent;

private:
	// Replicated list of equipment entries
	UPROPERTY()
	TArray<FAppliedEquipmentEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FEquipmentList> : public TStructOpsTypeTraitsBase2<FEquipmentList>
{
	enum { WithNetDeltaSerializer = true };
};


/* 아이템에 대한 장착 / 탈착을 관리하는 매니저 컴포넌트입니다. */
UCLASS(BlueprintType, ClassGroup = (Equipment), meta = (BlueprintSpawnableComponent))
class PROJECTV_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEquipmentComponent();

public:
	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void ReadyForReplication() override;

public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	UEquipmentInstance* EquipItem(TSubclassOf<UEquipmentDefinition> EquipmentDefinition);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void UnequipItem(UEquipmentInstance* ItemInstance);

	/* 첫번째로 찾은 장비 인스턴스를 반환합니다. 실패시 nullptr 입니다. */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UEquipmentInstance* GetFirstInstanceOfType(TSubclassOf<UEquipmentInstance> InstanceType);

 	/* 찾은 장비 인스턴스를 모두 반환합니다. 실패시 nullptr 입니다. */
 	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UEquipmentInstance*> GetEquipmentInstancesOfType(TSubclassOf<UEquipmentInstance> InstanceType) const;

	template <typename T>
	T* GetFirstInstanceOfType()
	{
		return (T*)GetFirstInstanceOfType(T::StaticClass());
	}

private:
	UPROPERTY(Replicated)
	FEquipmentList EquipmentList;
};

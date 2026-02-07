// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "SkinComponent.generated.h"

class ASkinActor;
class UChildActorComponent;

struct FNetDeltaSerializeInfo;

#pragma region Memo
/*
*	SkinActor에 Tag를 추가..? -> SkinPart.{ Head... FullMesh... } 하여 파츠별 장착 고려
*	동일 Tag에 해당하는 스킨들은 무조건 하나만 허용됨
*/
#pragma endregion


/* 적용된 스킨 하나의 정보를 담고있습니다. */
USTRUCT(BlueprintType)
struct FSkinPartEntry : public FFastArraySerializerItem 
{
	GENERATED_BODY()

public:
	FSkinPartEntry() : SkinActorClass(nullptr), SpawnedComponent(nullptr) { }

private:
	friend FSkinPartList;
	friend USkinComponent;

private:
	UPROPERTY(NotReplicated)
	FGameplayTag TypeTag;

	UPROPERTY()
	TSubclassOf<ASkinActor> SkinActorClass;

	UPROPERTY()
	FName Socket;

	// 복제시 생성되어 각자의 클라이언트에서 가지고 있을 정보이므로 복제하지 않음
	UPROPERTY(NotReplicated)
	TObjectPtr<UChildActorComponent> SpawnedComponent;
};

/* 적용된 스킨들을 관리하는 컨테이너입니다. */
USTRUCT(BlueprintType)
struct FSkinPartList : public FFastArraySerializer
{
	GENERATED_BODY()

public:
	FSkinPartList() : OwnerComponent(nullptr) { }
	FSkinPartList(UActorComponent* InOwnerComponent) : OwnerComponent(InOwnerComponent) { }

public:
	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	void AddEntry(FGameplayTag TypeTag, TSubclassOf<ASkinActor> SkinActorClass, FName Socket);
	void RemoveEntry(FGameplayTag TypeTag);

	FSkinPartEntry* GetEntry(FGameplayTag TypeTag);

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FSkinPartEntry, FSkinPartList>(Entries, DeltaParms, *this);
	}

private:
	void SpawnSkinActor(FSkinPartEntry& Entry);
	void DestroySkinActor(FSkinPartEntry& Entry);

private:
	friend USkinComponent;

private:
	UPROPERTY()
	TArray<FSkinPartEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FSkinPartList> : public TStructOpsTypeTraitsBase2<FSkinPartList>
{
	enum { WithNetDeltaSerializer = true };
};


/* 캐릭터의 스킨을 장착하거나 해제하는 역할을 수행하는 컴포넌트입니다. */
UCLASS(BlueprintType, ClassGroup = (Skin), meta = (BlueprintSpawnableComponent))
class PROJECTV_API USkinComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USkinComponent();

protected:
	virtual void BeginPlay() override;
	virtual void OnRegister() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION(BlueprintCallable, Category = "Skin Component", meta = (Categories = "Skin"))
	void SetSkin(TSubclassOf<ASkinActor> SkinActorClass, FName Socket);

	UFUNCTION(BlueprintPure, Category = "Skin Component", meta = (Categories = "Skin"))
	bool HasSkin(FGameplayTag TypeTag);

private:
	UPROPERTY(Replicated)
	FSkinPartList SkinList;
};

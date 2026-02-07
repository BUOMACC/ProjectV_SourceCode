// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/World.h"

#include "EquipmentInstance.generated.h"

class AActor;
class ACharacter;
struct FFrame;
struct FEquipmentActorToSpawn;

/**
 *	아이템을 장착했을때 생성되는 장비 인스턴스입니다.
 */
UCLASS(BlueprintType, Blueprintable)
class UEquipmentInstance : public UObject
{
	GENERATED_BODY()

public:
	UEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UObject interface
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual UWorld* GetWorld() const override final;
	//~End of UObject interface

	UFUNCTION(BlueprintPure, Category=Equipment)
	UObject* GetInstigator() const { return Instigator; }

	void SetInstigator(UObject* InInstigator) { Instigator = InInstigator; }

	UFUNCTION(BlueprintPure, Category=Equipment)
	ACharacter* GetCharacter() const;

	UFUNCTION(BlueprintPure, Category=Equipment)
	TArray<AActor*> GetSpawnedActors() const { return SpawnedActors; }

	virtual void SpawnEquipmentActors(const TArray<FEquipmentActorToSpawn>& ActorsToSpawn);
	virtual void DestroyEquipmentActors();

	virtual void OnEquipped();
	virtual void OnUnequipped();

protected:
	UFUNCTION(BlueprintImplementableEvent, Category=Equipment, meta=(DisplayName="On Equipped"))
	void K2_OnEquipped();

	UFUNCTION(BlueprintImplementableEvent, Category=Equipment, meta=(DisplayName="On Unequipped"))
	void K2_OnUnequipped();

private:
	UFUNCTION()
	void OnRep_Instigator();

protected:
	// 장착시 적용될 애니메이션입니다.
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TSubclassOf<UAnimInstance> EquippedAnimLayer;

	// 장착해제시 적용될 애니메이션입니다. (※ 주로 맨손 애니메이션 레이어를 넣습니다.)
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TSubclassOf<UAnimInstance> UnEquippedAnimLayer;

	//// 장착시 부여될 어빌리티 데이터입니다.
	//UPROPERTY(EditDefaultsOnly, Category = Equipment)
	//TSubclassOf<UGrantAbilityData> EquippedGrantAbilities;

	//// 장착해제시 부여될 어빌리티 데이터입니다. (※ 주로 맨손 어빌리티 데이터를 넣습니다.)
	//UPROPERTY(EditDefaultsOnly, Category = Equipment)
	//TSubclassOf<UGrantAbilityData> UnEquippedGrantAbilities;

private:
	/* 주로 Inventory Item Instance가 할당됨 */
	UPROPERTY(ReplicatedUsing = OnRep_Instigator)
	TObjectPtr<UObject> Instigator;

	UPROPERTY(Replicated)
	TArray<TObjectPtr<AActor>> SpawnedActors;
};

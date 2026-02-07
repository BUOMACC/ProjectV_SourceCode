// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Templates/SubclassOf.h"

#include "EquipmentDefinition.generated.h"

class AActor;
class UGrantAbilityData;
class UEquipmentInstance;

USTRUCT()
struct FEquipmentActorToSpawn
{
	GENERATED_BODY()

	FEquipmentActorToSpawn()
	{}

	UPROPERTY(EditAnywhere, Category=Equipment)
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, Category=Equipment)
	FName AttachSocket;

	UPROPERTY(EditAnywhere, Category=Equipment)
	FTransform AttachTransform;
};


/**
 *	장착 가능한 아이템에 대한 세부데이터 정의
 */
UCLASS(Blueprintable, Const, Abstract, BlueprintType)
class UEquipmentDefinition : public UObject
{
	GENERATED_BODY()

public:
	UEquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// 생성할 인스턴스 클래스입니다.
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TSubclassOf<UEquipmentInstance> InstanceType;

	// 장착시 부여될 어빌리티 데이터입니다.
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TArray<TObjectPtr<const UGrantAbilityData>> EquipAbilityToGrant;

	// 장착해제시 부여될 어빌리티 데이터입니다.
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TObjectPtr<const UGrantAbilityData> UnEquipAbilityToGrant;

	// 장착시 스폰될 액터 정보입니다.
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TArray<FEquipmentActorToSpawn> ActorsToSpawn;
};

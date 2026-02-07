// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractionInterface.h"
#include "PickupActor.generated.h"

class UItemDefinition;
class UGameplayAbility;

USTRUCT(BlueprintType)
struct PROJECTV_API FPickupInfo
{
	GENERATED_BODY()

public:
	FPickupInfo()
		: ItemDef(nullptr)
		, Count(0)
	{ }

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	TSubclassOf<UItemDefinition> ItemDef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "0"))
	int32 Count;
};

UCLASS()
class PROJECTV_API APickupActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	APickupActor();

protected:
	virtual void BeginPlay() override;

	// IInteractionInterface
	virtual bool CanInteract_Implementation(AActor* OwnerInstigator, AActor* AvatarActor) const override;
	virtual FInteractionInfo GetInteractionInfo() const override;
	// ~IInteractionInterface

public:
	const TArray<FPickupInfo>& GetPickupInfos() const;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	FInteractionInfo InteractionInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	TArray<FPickupInfo> PickupInfos;
};

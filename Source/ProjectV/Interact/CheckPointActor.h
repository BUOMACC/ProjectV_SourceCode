// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractionInterface.h"
#include "CheckPointActor.generated.h"

UCLASS()
class PROJECTV_API ACheckPointActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	ACheckPointActor();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// IInteractionInterface
	virtual void OnInteract_Implementation(AActor* OwnerInstigator, AActor* AvatarActor) override;
	virtual void OnInteractFinished_Implementation(AActor* OwnerInstigator, AActor* AvatarActor) override;
	virtual bool CanInteract_Implementation(AActor* OwnerInstigator, AActor* AvatarActor) const override;
	virtual FInteractionInfo GetInteractionInfo() const override;
	// ~IInteractionInterface

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CheckPoint", meta = (AllowPrivateAccess = "true"))
	FInteractionInfo InteractionInfo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Replicated, Category = "CheckPoint", meta = (AllowPrivateAccess = "true"))
	bool bIsUsed;
};

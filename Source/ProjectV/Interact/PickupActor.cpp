// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupActor.h"

APickupActor::APickupActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void APickupActor::BeginPlay()
{
	Super::BeginPlay();
}

bool APickupActor::CanInteract_Implementation(AActor* OwnerInstigator, AActor* AvatarActor) const
{
	return true;
}

FInteractionInfo APickupActor::GetInteractionInfo() const
{
	return InteractionInfo;
}

const TArray<FPickupInfo>& APickupActor::GetPickupInfos() const
{
	return PickupInfos;
}
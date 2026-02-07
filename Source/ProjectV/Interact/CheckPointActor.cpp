// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckPointActor.h"
#include "FunctionLibrary/CommonFunctionLibrary.h"
#include "Net/UnrealNetwork.h"

ACheckPointActor::ACheckPointActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bIsUsed = false;
	bReplicates = true;
}

void ACheckPointActor::BeginPlay()
{
	Super::BeginPlay();
}

void ACheckPointActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, bIsUsed);
}

void ACheckPointActor::OnInteract_Implementation(AActor* OwnerInstigator, AActor* AvatarActor)
{
	bIsUsed = true;
}

void ACheckPointActor::OnInteractFinished_Implementation(AActor* OwnerInstigator, AActor* AvatarActor)
{
	bIsUsed = false;
}

bool ACheckPointActor::CanInteract_Implementation(AActor* OwnerInstigator, AActor* AvatarActor) const
{
	if (bIsUsed)
	{
		return false;
	}

	if (IsValid(AvatarActor))
	{
		// 의자의 뒤가 아닌 정면에서만 사용 가능
		float Angle = UCommonFunctionLibrary::GetTwoActorAngle(this, AvatarActor, true);
		if (Angle > 60.f)
		{
			return false;
		}
	}

	return true;
}

FInteractionInfo ACheckPointActor::GetInteractionInfo() const
{
	return InteractionInfo;
}



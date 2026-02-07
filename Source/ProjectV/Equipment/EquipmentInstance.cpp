// Copyright Epic Games, Inc. All Rights Reserved.

#include "EquipmentInstance.h"
#include "Character/CombatCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "EquipmentDefinition.h"
#include "Net/UnrealNetwork.h"

class FLifetimeProperty;
class UClass;
class USceneComponent;

UEquipmentInstance::UEquipmentInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UWorld* UEquipmentInstance::GetWorld() const
{
	if (APawn* OwningPawn = GetCharacter())
	{
		return OwningPawn->GetWorld();
	}
	else
	{
		return nullptr;
	}
}

void UEquipmentInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Instigator);
	DOREPLIFETIME(ThisClass, SpawnedActors);
}

ACharacter* UEquipmentInstance::GetCharacter() const
{
	return Cast<ACharacter>(GetOuter());
}

void UEquipmentInstance::SpawnEquipmentActors(const TArray<FEquipmentActorToSpawn>& ActorsToSpawn)
{
	ACharacter* Character = GetCharacter();
	USceneComponent* AttachTarget = Character->GetMesh();

	for (const FEquipmentActorToSpawn& SpawnInfo : ActorsToSpawn)
	{
		AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(SpawnInfo.ActorToSpawn, FTransform::Identity, Character);
		NewActor->FinishSpawning(FTransform::Identity, /*bIsDefaultTransform=*/ true);
		NewActor->SetActorRelativeTransform(SpawnInfo.AttachTransform);
		NewActor->AttachToComponent(AttachTarget, FAttachmentTransformRules::KeepRelativeTransform, SpawnInfo.AttachSocket);

		SpawnedActors.Add(NewActor);
	}
}

void UEquipmentInstance::DestroyEquipmentActors()
{
	for (AActor* Actor : SpawnedActors)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}
}

void UEquipmentInstance::OnEquipped()
{
	check(GetCharacter());
	
	if (EquippedAnimLayer)
	{
		if (USkeletalMeshComponent* Mesh = GetCharacter()->GetMesh())
		{
			if (UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
			{
				AnimInstance->LinkAnimClassLayers(EquippedAnimLayer);
			}
		}
	}

	K2_OnEquipped();
}

void UEquipmentInstance::OnUnequipped()
{
	check(GetCharacter());

	if (UnEquippedAnimLayer)
	{
		if (USkeletalMeshComponent* Mesh = GetCharacter()->GetMesh())
		{
			if (UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
			{
				AnimInstance->LinkAnimClassLayers(UnEquippedAnimLayer);
			}
		}
	}

	K2_OnUnequipped();
}

void UEquipmentInstance::OnRep_Instigator()
{
}


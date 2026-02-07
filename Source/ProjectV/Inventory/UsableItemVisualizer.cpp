// Fill out your copyright notice in the Description page of Project Settings.


#include "UsableItemVisualizer.h"
#include "Components/StaticMeshComponent.h"
#include "Inventory/ItemInstance.h"
#include "Inventory/Fragment/ItemFragment_Useable.h"

// Sets default values
AUsableItemVisualizer::AUsableItemVisualizer()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	SetActorEnableCollision(false);

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
	VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	VisualMesh->SetIsReplicated(true);
}

void AUsableItemVisualizer::SetVisual(UItemInstance* InItemInstance)
{
	if (IsValid(InItemInstance))
	{
		if (const UItemFragment_Useable* Fragment = InItemInstance->FindItemFragmentByClass<UItemFragment_Useable>())
		{
			if (Fragment->VisualMesh)
			{
				VisualMesh->SetStaticMesh(Fragment->VisualMesh);
			}
		}
	}
}

UStaticMeshComponent* AUsableItemVisualizer::GetVisualMesh() const
{
	return VisualMesh;
}

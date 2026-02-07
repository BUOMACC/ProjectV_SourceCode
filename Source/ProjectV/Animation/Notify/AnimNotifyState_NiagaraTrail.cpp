// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_NiagaraTrail.h"
#include "Equipment/EquipmentComponent.h"
#include "Equipment/EquipmentInstance.h"
#include "GameFramework/Character.h"
#include "Interface/WeaponTrailInterface.h"
#include "NiagaraComponent.h"

UAnimNotifyState_NiagaraTrail::UAnimNotifyState_NiagaraTrail()
{
#if WITH_EDITORONLY_DATA
	bShouldFireInEditor = false;
#endif

	TrailStartParameterName = TEXT("Trail_Start");
	TrailEndParameterName = TEXT("Trail_End");
}

void UAnimNotifyState_NiagaraTrail::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner()))
	{
		if (UNiagaraComponent* NiagaraComponent = Cast<UNiagaraComponent>(GetSpawnedEffect(MeshComp)))
		{
			// EquipmentComponent가 있다면 -> 장착한 장비액터에게 발생시킴
			if (UEquipmentComponent* EquipmentComponent = Character->FindComponentByClass<UEquipmentComponent>())
			{
				const TArray<UEquipmentInstance*> Equipments = EquipmentComponent->GetEquipmentInstancesOfType(UEquipmentInstance::StaticClass());
				for (UEquipmentInstance* Equipment : Equipments)
				{
					for (AActor* SpawnedActor : Equipment->GetSpawnedActors())
					{
						SetTrail(SpawnedActor, NiagaraComponent);
					}
				}
			}
			// EquipmentComponent를 찾지 못했다면 -> Owner한테 발생시킴
			else
			{
				SetTrail(Character, NiagaraComponent);
			}
		}
	}
}

void UAnimNotifyState_NiagaraTrail::SetTrail(AActor* TargetActor, UNiagaraComponent* NiagaraComponent)
{
	if (TargetActor && TargetActor->GetClass()->ImplementsInterface(UWeaponTrailInterface::StaticClass()))
	{
		FVector TrailStart = FVector::ZeroVector;
		FVector TrailEnd = FVector::ZeroVector;

		// get the trail start & end point
		IWeaponTrailInterface::Execute_GetTrailPoint(TargetActor, TrailStart, TrailEnd);

		if (NiagaraComponent)
		{
			NiagaraComponent->SetVectorParameter(TrailStartParameterName, TrailStart);
			NiagaraComponent->SetVectorParameter(TrailEndParameterName, TrailEnd);
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_PlayFootstepFX.h"
#include "Character/CombatCharacter.h"
#include "Data/FootstepData.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

UAnimNotify_PlayFootstepFX::UAnimNotify_PlayFootstepFX()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(0, 210, 50);
#endif

	VolumeMultiplier = 1.f;
	PitchMultiplier = 1.f;
	BoneName = TEXT("foot_l");
	GroundTraceChannel = ETraceTypeQuery::TraceTypeQuery1;
	bIgnoreVFX = false;
}

FString UAnimNotify_PlayFootstepFX::GetNotifyName_Implementation() const
{
	return TEXT("PlayFootstepFX");
}

void UAnimNotify_PlayFootstepFX::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ACombatCharacter* CombatCharacter = Cast<ACombatCharacter>(MeshComp->GetOwner());
	if (CombatCharacter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANimNotify_PlayFootstepFX::Notify Failed. -> Meshcomp Owner is not ACombatCharacter Type"));
		return;
	}

	UFootstepData* FootstepData = CombatCharacter->FootstepData;
	if (FootstepData == nullptr)
	{
		return;
	}

	// Get Floor Surface Type
	if (UWorld* World = CombatCharacter->GetWorld())
	{
		FHitResult HitResult;
		TArray<AActor*> Ignore;
		Ignore.Add(CombatCharacter);

		const FVector& Start = CombatCharacter->GetFootLocation();
		const FVector& End = Start + (FVector::DownVector * 30.f /* hard coded */);

		if (UKismetSystemLibrary::LineTraceSingle(World, Start, End, GroundTraceChannel, false, Ignore, EDrawDebugTrace::None, HitResult, true)
			&& HitResult.bBlockingHit)
		{
			TWeakObjectPtr<UPhysicalMaterial> PhysicalMaterial = HitResult.PhysMaterial;
			if (PhysicalMaterial.IsValid())
			{
				if (USoundBase* FootstepSound = FootstepData->FindFootstepSound(PhysicalMaterial->SurfaceType))
				{
					UGameplayStatics::PlaySoundAtLocation(World, FootstepSound, HitResult.ImpactPoint, VolumeMultiplier, PitchMultiplier);
				}

				if (!bIgnoreVFX)
				{
					if (UNiagaraSystem* FootstepEffect = FootstepData->FindFootstepEffect(PhysicalMaterial->SurfaceType))
					{
						if (USkeletalMeshComponent* Mesh = CombatCharacter->GetMesh())
						{
							FVector EffectLocation = Mesh->GetSocketLocation(BoneName);
							UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, FootstepEffect, EffectLocation, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true);
						}
					}
				}
			}
		}
	}
}
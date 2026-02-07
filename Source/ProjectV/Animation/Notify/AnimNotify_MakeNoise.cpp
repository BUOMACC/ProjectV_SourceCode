// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_MakeNoise.h"
#include "GameFramework/Character.h"

UAnimNotify_MakeNoise::UAnimNotify_MakeNoise()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(150, 150, 150);
#endif

	Loudness = 1.f;
	Range = 1000.f;
}

FString UAnimNotify_MakeNoise::GetNotifyName_Implementation() const
{
	return TEXT("MakeNoise");
}

void UAnimNotify_MakeNoise::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation);

	if (AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerActor))
		{
			OwnerCharacter->MakeNoise(Loudness, OwnerCharacter, OwnerCharacter->GetActorLocation(), Range);
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Ragdoll.h"
#include "Character/CombatCharacter.h"
#include "FunctionLibrary/CommonFunctionLibrary.h"

UAnimNotify_Ragdoll::UAnimNotify_Ragdoll()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(255, 0, 0);
#endif

	RandomImpulseMin = FVector::ZeroVector;
	RandomImpulseMax = FVector::ZeroVector;
}

FString UAnimNotify_Ragdoll::GetNotifyName_Implementation() const
{
	return TEXT("Ragdoll");
}

void UAnimNotify_Ragdoll::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AActor* Actor = MeshComp->GetOwner())
	{
		if (ACombatCharacter* CombatCharacter = Cast<ACombatCharacter>(Actor))
		{
			FVector Impulse = UCommonFunctionLibrary::RandomVectorRange(RandomImpulseMin, RandomImpulseMax);
			CombatCharacter->Ragdoll(Impulse);
		}
	}
}

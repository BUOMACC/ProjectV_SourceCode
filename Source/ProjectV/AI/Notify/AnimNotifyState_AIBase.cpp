// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_AIBase.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/Character.h"

UAnimNotifyState_AIBase::UAnimNotifyState_AIBase()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(150, 150, 150);
#endif
}

FString UAnimNotifyState_AIBase::GetNotifyName_Implementation() const
{
	return TEXT("AI Base");
}

void UAnimNotifyState_AIBase::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!CheckAI(MeshComp->GetOwner())) return;
	NotifyBeginAI(MeshComp, Animation, TotalDuration, EventReference);
}

void UAnimNotifyState_AIBase::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (!CheckAI(MeshComp->GetOwner())) return;
	NotifyTickAI(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UAnimNotifyState_AIBase::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!CheckAI(MeshComp->GetOwner())) return;
	NotifyEndAI(MeshComp, Animation, EventReference);
}

void UAnimNotifyState_AIBase::NotifyBeginAI(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	// not implemented.
}

void UAnimNotifyState_AIBase::NotifyTickAI(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	// not implemented.
}

void UAnimNotifyState_AIBase::NotifyEndAI(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	// not implemented.
}

UAbilitySystemComponent* UAnimNotifyState_AIBase::GetOwningAICharacterASC(USkeletalMeshComponent* MeshComp) const
{
	if (AActor* OwningActor = GetOwningAI<AActor>(MeshComp))
	{
		return UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor);
	}
	return nullptr;
}

bool UAnimNotifyState_AIBase::CheckAI(AActor* Actor) const
{
	if (Actor == nullptr || !Actor->HasAuthority())
		return false;

	if (ACharacter* Character = Cast<ACharacter>(Actor))
	{
		if (Character->IsPlayerControlled())
		{
			UE_LOG(LogTemp, Warning, TEXT("UAnimNotifyState_AIBase::CheckAI() - 해당 함수는 AI에서만 동작합니다."));
			return false;
		}
		return true;
	}
	return false;
}
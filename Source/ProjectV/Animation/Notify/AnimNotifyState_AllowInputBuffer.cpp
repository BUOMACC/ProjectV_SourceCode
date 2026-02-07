// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_AllowInputBuffer.h"
#include "Character/PlayerCharacter.h"
#include "Character/PlayerComponent.h"

UAnimNotifyState_AllowInputBuffer::UAnimNotifyState_AllowInputBuffer()
{
#if WITH_EDITORONLY_DATA
	// TODO: Setting Color Here
#endif
}

FString UAnimNotifyState_AllowInputBuffer::GetNotifyName_Implementation() const
{
	return TEXT("AllowInputBuffer");
}

void UAnimNotifyState_AllowInputBuffer::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	// Only Player
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner()))
	{
		// 컨트롤중인 캐릭터가 아니면 진행안함
		if (Player->IsLocallyControlled() == false)
		{
			return;
		}

		if (UPlayerComponent* PlayerComp = Player->GetPlayerComponent())
		{
			PlayerComp->EnableInputBuffer();
		}
	}
}

void UAnimNotifyState_AllowInputBuffer::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	// Only Player
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner()))
	{
		// 컨트롤중인 캐릭터가 아니면 진행안함
		if (Player->IsLocallyControlled() == false)
		{
			return;
		}

		if (UPlayerComponent* PlayerComp = Player->GetPlayerComponent())
		{
			PlayerComp->ClearInputBuffer();
			PlayerComp->DisableInputBuffer();
		}
	}
}
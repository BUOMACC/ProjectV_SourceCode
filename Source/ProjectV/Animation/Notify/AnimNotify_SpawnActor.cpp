// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_SpawnActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "CombatGameplayTags.h"
#include "Engine/World.h"

UAnimNotify_SpawnActor::UAnimNotify_SpawnActor()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(252, 198, 3);
#endif

	bRunOnServer = false;
	ActorToSpawn = nullptr;
	bAttachToSocket = false;
	AttachSocketName = NAME_None;
	SpawnMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	RelativeTransform = FTransform::Identity;
	bSendGameplayEvent = false;
}

void UAnimNotify_SpawnActor::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	AActor* OwnerActor = MeshComp->GetOwner();
	if (!IsValid(OwnerActor))
		return;

	UWorld* World = OwnerActor->GetWorld();

	/* Editor Preview */
#if WITH_EDITOR
	float NotifyTiming = EventReference.GetNotify()->GetTime(EAnimLinkMethod::Absolute);
	if (World && World->WorldType == EWorldType::EditorPreview)
	{
		AActor* SpawnedActor = SpawnActor(World, MeshComp, OwnerActor);
		/* 스폰하지 못하거나, 도중에 파괴될 위험이 있기에 체크 */
		if (IsValid(SpawnedActor))
		{
			SpawnedActor->SetLifeSpan(Animation->GetPlayLength() - NotifyTiming);
		}
		return;
	}
#endif

	/* In Game */
	if (bRunOnServer)
	{
		if (OwnerActor->HasAuthority())
		{
			SpawnActor(World, MeshComp, OwnerActor);
		}
	}
	else
	{
		SpawnActor(World, MeshComp, OwnerActor);
	}
}

FString UAnimNotify_SpawnActor::GetNotifyName_Implementation() const
{
	return TEXT("SpawnActor");
}

AActor* UAnimNotify_SpawnActor::SpawnActor(UWorld* World, USkeletalMeshComponent* MeshComp, AActor* OwnerActor)
{
	if (ActorToSpawn == nullptr || World == nullptr)
		return nullptr;

	FTransform SpawnTransform = FTransform::Identity;
	if (bAttachToSocket)
	{
		SpawnTransform = MeshComp->GetSocketTransform(AttachSocketName, ERelativeTransformSpace::RTS_World);
	}
	else
	{
		SpawnTransform = MeshComp->GetComponentTransform();
	}


	SpawnTransform *= RelativeTransform;

	FActorSpawnParameters Param;
	Param.Owner = OwnerActor;
	Param.SpawnCollisionHandlingOverride = SpawnMethod;

	AActor* SpawnedActor = World->SpawnActor<AActor>(ActorToSpawn, SpawnTransform, Param);
	if (SpawnedActor && bAttachToSocket)
	{
		/* 위에서 이미 어태치될 본에 대한 좌표로 스폰되었으므로, 현재 위치를 자동으로 계산하도록 KeepWorldTransform 옵션을 사용합니다. */
		SpawnedActor->AttachToComponent(MeshComp, FAttachmentTransformRules::KeepWorldTransform, AttachSocketName);
	}

	// Send Spawn Event
	if (bSendGameplayEvent)
	{
		FGameplayEventData Payload;
		Payload.Instigator = OwnerActor;
		Payload.OptionalObject = SpawnedActor;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, FCombatGameplayTags::Get().NotifyTiming_AfterSpawn, Payload);
	}

	return SpawnedActor;
}

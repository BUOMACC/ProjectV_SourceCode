// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_TimedSpawnActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "CombatGameplayTags.h"

UAnimNotifyState_TimedSpawnActor::UAnimNotifyState_TimedSpawnActor()
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

FString UAnimNotifyState_TimedSpawnActor::GetNotifyName_Implementation() const
{
	return TEXT("Timed SpawnActor");
}

void UAnimNotifyState_TimedSpawnActor::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AActor* OwnerActor = MeshComp->GetOwner();
	if (!IsValid(OwnerActor))
		return;

	UWorld* World = OwnerActor->GetWorld();

	/* Editor Preview */
#if WITH_EDITOR
	if (World && World->WorldType == EWorldType::EditorPreview)
	{
		AActor* SpawnedActor = SpawnActor(World, MeshComp, OwnerActor);
		/* 스폰하지 못하거나, 도중에 파괴될 위험이 있기에 체크 */
		if (IsValid(SpawnedActor))
		{
			SpawnedActor->SetLifeSpan(TotalDuration);
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

void UAnimNotifyState_TimedSpawnActor::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AActor* OwnerActor = MeshComp->GetOwner();
	if (!IsValid(OwnerActor))
		return;

	UWorld* World = OwnerActor->GetWorld();

	/* Editor Preview */
#if WITH_EDITOR
	if (World && World->WorldType == EWorldType::EditorPreview)
	{
		/* Lifespan을 설정하여 스폰했으므로 Destroy를 직접 할 필요 없음 */
		return;
	}

#endif

	/* In Game */
	if (bRunOnServer)
	{
		if (OwnerActor->HasAuthority())
		{
			DestroyActor(OwnerActor);
		}
	}
	else
	{
		DestroyActor(OwnerActor);
	}
}

AActor* UAnimNotifyState_TimedSpawnActor::SpawnActor(UWorld* World, USkeletalMeshComponent* MeshComp, AActor* OwnerActor)
{
	if (ActorToSpawn == nullptr || World == nullptr)
		return nullptr;

	FTransform SpawnTransform = FTransform::Identity;
	if (!bAttachToSocket)
	{
		SpawnTransform = MeshComp->GetComponentTransform();
		SpawnTransform *= RelativeTransform;
	}

	FActorSpawnParameters Param;
	Param.Owner = OwnerActor;
	Param.SpawnCollisionHandlingOverride = SpawnMethod;

	AActor* SpawnedActor = World->SpawnActor<AActor>(ActorToSpawn, SpawnTransform, Param);
	if (SpawnedActor)
	{
		FObjectKey Key(OwnerActor);

		/* 이전에 스폰된 액터가 있다면 파괴시킵니다. */
		DestroyActor(OwnerActor);

		/* NotifyState 시간이 지난 뒤 지울 수 있도록 맵에 추가합니다. */
		SpawnedActors.Add(Key, SpawnedActor);

		/* 위에서 이미 어태치될 본에 대한 좌표로 스폰되었으므로, 현재 위치를 자동으로 계산하도록 KeepWorldTransform 옵션을 사용합니다. */
		if (bAttachToSocket)
		{
			SpawnedActor->AttachToComponent(MeshComp, FAttachmentTransformRules::KeepRelativeTransform, AttachSocketName);
			SpawnedActor->SetActorRelativeTransform(RelativeTransform);
		}
	}

	// Send Gameplay Event
	if (bSendGameplayEvent)
	{
		FGameplayEventData Payload;
		Payload.Instigator = OwnerActor;
		Payload.OptionalObject = SpawnedActor;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, FCombatGameplayTags::Get().NotifyTiming_AfterSpawn, Payload);
	}

	return SpawnedActor;
}

void UAnimNotifyState_TimedSpawnActor::DestroyActor(AActor* OwnerActor)
{
	FObjectKey Key(OwnerActor);
	if (SpawnedActors.Contains(Key))
	{
		TWeakObjectPtr<AActor> Actor = SpawnedActors[Key];
		if (Actor.IsValid())
		{
			Actor->Destroy();
		}
	}
}

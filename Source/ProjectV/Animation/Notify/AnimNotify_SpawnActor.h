// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_SpawnActor.generated.h"

/**
 *	플레이어의 트랜스폼을 기준으로 액터를 스폰하는 노티파이
 */
UCLASS(meta = (DisplayName = "SpawnActor"))
class PROJECTV_API UAnimNotify_SpawnActor : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UAnimNotify_SpawnActor();

private:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	virtual FString GetNotifyName_Implementation() const override;

	AActor* SpawnActor(UWorld* World, USkeletalMeshComponent* MeshComp, AActor* OwnerActor);

private:
	/* 이 값이 true라면, 서버에서만 노티파이가 실행됩니다. */
	UPROPERTY(EditAnywhere, Category = "Spawn Actor", meta = (AllowPrivateAccess = "true"))
	bool bRunOnServer;

	/* 스폰될 액터입니다. */
	UPROPERTY(EditAnywhere, Category = "Spawn Actor", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> ActorToSpawn;

	/* 이 값이 true라면, 소켓에 어태치됩니다. */
	UPROPERTY(EditAnywhere, Category = "Spawn Actor", meta = (AllowPrivateAccess = "true"))
	bool bAttachToSocket;

	/* 어태치될 소켓 이름입니다. */
	UPROPERTY(EditAnywhere, Category = "Spawn Actor", meta = (AllowPrivateAccess = "true", EditCondition = "bAttachToSocket == true"))
	FName AttachSocketName;

	UPROPERTY(EditAnywhere, Category = "Spawn Actor", meta = (AllowPrivateAccess = "true"))
	ESpawnActorCollisionHandlingMethod SpawnMethod;

	/* 스폰위치를 기준으로 한 상대적 좌표입니다. */
	UPROPERTY(EditAnywhere, Category = "Spawn Actor", meta = (AllowPrivateAccess = "true"))
	FTransform RelativeTransform;

	/* 이 값이 true라면, 액터 스폰 완료 이벤트를 전송합니다. (NotifyTiming.AfterSpawn) */
	UPROPERTY(EditAnywhere, Category = "Spawn Actor|Event", meta = (AllowPrivateAccess = "true"))
	bool bSendGameplayEvent;
};

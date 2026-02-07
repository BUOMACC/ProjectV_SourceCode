// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_TimedSpawnActor.generated.h"

/**
 *	NotifyState 범위동안에만 유효한 액터를 스폰입니다.
 *	NotifyState가 끝나면 액터가 파괴됩니다.
 */
UCLASS(meta = (DisplayName = "Timed SpawnActor"))
class PROJECTV_API UAnimNotifyState_TimedSpawnActor : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UAnimNotifyState_TimedSpawnActor();

private:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	AActor* SpawnActor(UWorld* World, USkeletalMeshComponent* MeshComp, AActor* OwnerActor);
	void DestroyActor(AActor* OwnerActor);

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

private:
	/*
	*	Notify는 기본적으로 Instanced로 되어있어 Notify끼리는 별도의 인스턴스로 각각 구분이 가능한 반면,
	*	같은 Montage를 실행하는 경우 Notify는 동일 인스턴스로 데이터를 공유하게 됩니다.
	*	따라서 다른 플레이어가 생성한 액터를 본인이 지울수도 있어 이를 구분하기 위해 맵으로 관리합니다.
	*/
	TMap<FObjectKey, TWeakObjectPtr<AActor>> SpawnedActors;
};

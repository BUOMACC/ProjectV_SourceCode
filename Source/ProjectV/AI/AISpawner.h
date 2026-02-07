// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AISpawner.generated.h"

class AEnemyCharacter;
class APatrolPathActor;

/* 스폰 대상과 가중치에 대해 지정할 수 있는 구조체입니다. */
USTRUCT(BlueprintType)
struct PROJECTV_API FAISpawnProb
{
	GENERATED_BODY()

public:
	FAISpawnProb();

public:
	/* 스폰할 적 캐릭터 클래스입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<AEnemyCharacter> EnemyClass;

	/* 스폰 가중치입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnProb;
};

/* 스폰할 AI에 대한 정보를 나타내는 구조체입니다. */
USTRUCT(BlueprintType)
struct PROJECTV_API FAISpawnInfo
{
	GENERATED_BODY()

public:
	FAISpawnInfo();

public:
	void FillUsableTransform();
	bool TakeSpawnTransform(FTransform& OutLocalTransform);

public:
	/* 스폰할 대상에 대한 정보입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAISpawnProb> SpawnProbs;

	/* 적용될 순찰 경로입니다. 비어있는 경우 순찰 경로가 적용되지 않습니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APatrolPathActor> PatrolPath;

	/* 스폰될 위치입니다. 해당 위치 중 사용중이 아닌 랜덤한 곳에 스폰됩니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MakeEditWidget))
	TArray<FTransform> SpawnTransforms;

private:
	#pragma region Memo
	/*
	*  Queue, Stack을 사용하지 않았습니다. 무작위로 삽입되는 데이터 특성이
	*  해당 자료형의 용도(FIFO, LIFO)를 무색하게 만들 수 있기 때문입니다.
	*/
	#pragma endregion
	/* 사용 가능한 무작위로 섞인 좌표 목록입니다. 사용된 좌표는 리스트에서 삭제됩니다. */
	TArray<FTransform> UsableSpawnTransforms;
};

/* 스폰된 이후 AI 정보를 추적하는 구조체입니다. */
USTRUCT(BlueprintType)
struct PROJECTV_API FActiveAIInfo
{
	GENERATED_BODY()

public:
	FActiveAIInfo();

public:
	/* 스폰된 대상의 인스턴스입니다. 파괴된 경우 nullptr일 수 있습니다. */
	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<AEnemyCharacter> EnemyInstance;

	/* 스폰된 대상의 클래스입니다. */
	UPROPERTY(VisibleInstanceOnly)
	TSubclassOf<AEnemyCharacter> EnemyClass;

	/* 스폰된 시점의 위치입니다. */
	UPROPERTY(VisibleInstanceOnly)
	FVector SpawnedLocation;

	/* 대상이 죽었는지 여부입니다. */
	UPROPERTY(VisibleInstanceOnly)
	bool bIsAlive;
};

/*
*  1) 테이블 스포너 관리기능			-> 후순위
*  2) 스포너 몬스터 전체 리스폰 기능
*  3) 스포너 Patrol Path 연결 기능
*  4) 스포너 딜레이 및 스폰 횟수 로직
*  5) 스폰 위치 지정 기능
*  6) 초기상태 및 팀 지정기능
*  7) 특정 범위에 들어야 AI가 활성화 되는 기능
*/

UCLASS()
class PROJECTV_API AAISpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AAISpawner();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

protected:
	/* 일반적인 스폰을 진행합니다. */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AI Spawner")
	void ProcessSpawn();

private:
	/* 대상이 스폰되어야 할 위치를 반환하고 이를 목록에서 제외합니다. */
	bool TakeBestWorldSpawnTransform(FAISpawnInfo& InSpawnInfo, FTransform& OutWorldTransform) const;

	/* Prob을 고려하여 랜덤한 스폰 대상 클래스를 반환합니다. */
	TSubclassOf<AEnemyCharacter> GetBestEnemyClass(const TArray<FAISpawnProb>& InSpawnProbs) const;

	/* 입력받은 Prob의 총 합을 반환합니다. */
	int32 GetTotalProb(const TArray<FAISpawnProb>& InProbs) const;

protected:
	UFUNCTION()
	void OnEnemyDead(ACombatCharacter* Victim);

	bool CanSpawnEnemy() const;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI Spawner")
	TArray<FAISpawnInfo> SpawnInfos;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI Spawner")
	TArray<FActiveAIInfo> ActiveInfos;
};

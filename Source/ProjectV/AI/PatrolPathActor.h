// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPathActor.generated.h"

class USplineComponent;

UENUM(BlueprintType)
enum class EPatrolPathRule : uint8
{
	/* 종료 위치에 도달하면 다음 위치는 없습니다. */
	Stop,

	/* 종료 위치에 도달하면 다음 위치는 다시 첫 위치입니다. */
	Cycle,

	/* 종료 위치에 도달하면 다음 위치는 이전 위치로 거슬러 갑니다. */
	Reverse
};

USTRUCT(BlueprintType)
struct PROJECTV_API FPatrolPathInfo
{
	GENERATED_BODY()

	FPatrolPathInfo() : Direction(1), Index(0) { }
	
public:
	void InverseDirection() { Direction *= -1; }

public:
	int32 Direction;
	int32 Index;
};

/*
*	AI의 순찰 포인트를 지정하는 액터
*/
UCLASS()
class PROJECTV_API APatrolPathActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APatrolPathActor();

	virtual void BeginPlay() override;

public:
	/* 입력받은 액터를 등록합니다. 등록이 완료되면 Patrol Path Info에 기록됩니다. */
	UFUNCTION(BlueprintCallable, Category = "Patrol Path Actor")
	bool RegisterActor(AActor* InActor);

	/* 입력받은 액터를 등록 해제시킵니다. */
	UFUNCTION(BlueprintCallable, Category = "Patrol Path Actor")
	bool UnregisterActor(AActor* InActor);

	/* 다음 순찰 포인트가 존재하는지 반환합니다.. */
	UFUNCTION(BlueprintPure, Category = "Patrol Path Actor")
	bool HasNextPatrolPoint(AActor* InActor);

	/* 다음 순찰 포인트로 변경합니다. */
	UFUNCTION(BlueprintCallable, Category = "Patrol Path Actor")
	bool AdvanceNextPatrolPoint(AActor* InActor, FVector& OutNextPatrolPoint);

	/* 현재 순찰 포인트를 반환합니다. */
	UFUNCTION(BlueprintPure, Category = "Patrol Path Actor")
	bool GetPatrolPoint(AActor* InActor, FVector& OutPatrolPoint);
	
protected:
	FPatrolPathInfo* GetPatrolPathInfo(AActor* InActor);
	bool IsRegistered(AActor* InActorToCheck) const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USplineComponent> SplineComponent;

	/* 종료 위치에 도달했을때 다음 위치를 선정하는 규칙입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol Path Actor")
	EPatrolPathRule PatrolPathRule;

protected:
	/* 객체별로 현재 순찰 위치 정보입니다. */
	TMap<FObjectKey, FPatrolPathInfo> PatrolPathInfo;
};

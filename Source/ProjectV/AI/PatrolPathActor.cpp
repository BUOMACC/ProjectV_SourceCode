// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolPathActor.h"
#include "Components/SplineComponent.h"
#include "ProjectVLogs.h"

APatrolPathActor::APatrolPathActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	RootComponent = SplineComponent;
}

void APatrolPathActor::BeginPlay()
{
	Super::BeginPlay();
}

bool APatrolPathActor::RegisterActor(AActor* InActor)
{
	if (!IsValid(InActor))
		return false;

	if (IsRegistered(InActor))
	{
		UE_LOG(LogProjectV, Warning, TEXT("APatrolPathActor::RegisterActor - 이미 등록된 객체입니다. [%s]"), *InActor->GetName());
		return false;
	}

	// 입력받은 객체의 순찰 위치 정보를 등록합니다.
	FObjectKey Key(InActor);
	PatrolPathInfo.Add(Key, FPatrolPathInfo());
	UE_LOG(LogProjectV, Log, TEXT("APatrolPathActor::RegisterActor - [%s] 객체의 순찰 위치 정보가 등록되었습니다."), *InActor->GetName());
	return true;
}

bool APatrolPathActor::UnregisterActor(AActor* InActor)
{
	if (!IsValid(InActor))
		return false;

	if (!IsRegistered(InActor))
	{
		UE_LOG(LogProjectV, Warning, TEXT("APatrolPathActor::UnregisterActor - 동록되지 않은 객체입니다. [%s]"), *InActor->GetName());
		return false;
	}

	// 입력받은 객체의 순찰 위치 정보를 등록합니다.
	FObjectKey Key(InActor);
	PatrolPathInfo.Remove(Key);
	return true;
}

bool APatrolPathActor::HasNextPatrolPoint(AActor* InActor)
{
	if (FPatrolPathInfo* PathInfo = GetPatrolPathInfo(InActor))
	{
		const int32 NumberOfPath = SplineComponent->GetNumberOfSplinePoints();
		switch (PatrolPathRule)
		{
		case EPatrolPathRule::Stop:
			return (PathInfo->Index + PathInfo->Direction) < NumberOfPath;

		case EPatrolPathRule::Cycle:
		case EPatrolPathRule::Reverse:
			return NumberOfPath > 1;
		}
	}
	return false;
}

bool APatrolPathActor::AdvanceNextPatrolPoint(AActor* InActor, FVector& OutNextPatrolPoint)
{
	if (FPatrolPathInfo* PathInfo = GetPatrolPathInfo(InActor))
	{
		const int32 NumberOfPath = SplineComponent->GetNumberOfSplinePoints();
		PathInfo->Index += PathInfo->Direction;

		switch (PatrolPathRule)
		{
		case EPatrolPathRule::Stop:
			if (PathInfo->Index >= NumberOfPath)
			{
				PathInfo->Index = NumberOfPath - 1;
				OutNextPatrolPoint = FVector::ZeroVector;
				return false;
			}
			break;

		case EPatrolPathRule::Cycle:
			PathInfo->Index %= NumberOfPath;
			break;

		case EPatrolPathRule::Reverse:
			if (PathInfo->Index >= NumberOfPath || PathInfo->Index < 0)
			{
				PathInfo->InverseDirection();
				// 위에서 값을 이미 더한 뒤이므로, Direction만큼 두번 이동해야 유효한 위치가 됩니다.
				PathInfo->Index += (PathInfo->Direction * 2); 
			}
			break;

		default:
			OutNextPatrolPoint = FVector::ZeroVector;
			return false;
		}
		const int32 AdjustedIndex = FMath::Clamp(PathInfo->Index, 0, NumberOfPath - 1);
		OutNextPatrolPoint = SplineComponent->GetLocationAtSplinePoint(AdjustedIndex, ESplineCoordinateSpace::Type::World);
		return true;
	}
	OutNextPatrolPoint = FVector::ZeroVector;
	return false;
}

bool APatrolPathActor::GetPatrolPoint(AActor* InActor, FVector& OutPatrolPoint)
{
	if (FPatrolPathInfo* PathInfo = GetPatrolPathInfo(InActor))
	{
		const int32 AdjustedIndex = FMath::Clamp(PathInfo->Index, 0, SplineComponent->GetNumberOfSplinePoints() - 1);
		OutPatrolPoint = SplineComponent->GetLocationAtSplinePoint(AdjustedIndex, ESplineCoordinateSpace::Type::World);
		return true;
	}
	OutPatrolPoint = FVector::ZeroVector;
	return false;
}

FPatrolPathInfo* APatrolPathActor::GetPatrolPathInfo(AActor* InActor)
{
	if (!IsValid(InActor))
		return nullptr;

	FObjectKey Key(InActor);
	if (FPatrolPathInfo* FoundInfo = PatrolPathInfo.Find(Key))
	{
		return FoundInfo;
	}

	return nullptr;
}

bool APatrolPathActor::IsRegistered(AActor* InActorToCheck) const
{
	if (!IsValid(InActorToCheck))
		return false;

	FObjectKey Key(InActorToCheck);
	return PatrolPathInfo.Contains(Key);
}

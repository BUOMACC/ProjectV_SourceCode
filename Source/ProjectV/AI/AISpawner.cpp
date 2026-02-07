// Fill out your copyright notice in the Description page of Project Settings.


#include "AISpawner.h"
#include "Algo/RandomShuffle.h"
#include "Character/EnemyCharacter.h"
#include "Development/CombatDeveloperSettings.h"
#include "ProjectVLogs.h"
#include "Kismet/KismetMathLibrary.h"

FAISpawnProb::FAISpawnProb()
	: EnemyClass(nullptr), SpawnProb(1)
{
	// ...
}


FAISpawnInfo::FAISpawnInfo()
	: SpawnProbs(TArray<FAISpawnProb>()), PatrolPath(nullptr), SpawnTransforms(TArray<FTransform>()), UsableSpawnTransforms(TArray<FTransform>())
{
	// ...
}

void FAISpawnInfo::FillUsableTransform()
{
	UsableSpawnTransforms = SpawnTransforms;
	Algo::RandomShuffle(UsableSpawnTransforms);
}

bool FAISpawnInfo::TakeSpawnTransform(FTransform& OutLocalTransform)
{
	if (UsableSpawnTransforms.Num() == 0)
	{
		UE_LOG(LogAISpawner, Warning, TEXT("스폰을 위한 위치를 찾지 못했습니다. 사용 가능한 위치가 부족합니다."))
		OutLocalTransform = FTransform::Identity;
		return false;
	}

	OutLocalTransform = UsableSpawnTransforms[0];
	UsableSpawnTransforms.RemoveAt(0);
	return true;
}

FActiveAIInfo::FActiveAIInfo()
	: EnemyInstance(nullptr), EnemyClass(nullptr), SpawnedLocation(FVector::ZeroVector), bIsAlive(false)
{
	// ...
}


AAISpawner::AAISpawner()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AAISpawner::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	for (FAISpawnInfo& SpawnInfo : SpawnInfos)
	{
		SpawnInfo.FillUsableTransform();
	}
}

void AAISpawner::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() && CanSpawnEnemy())
	{
		ProcessSpawn();
	}
}

void AAISpawner::ProcessSpawn()
{
	if (HasAuthority() == false)
	{
		return;
	}

	for (FAISpawnInfo& SpawnInfo : SpawnInfos)
	{
		TSubclassOf<AEnemyCharacter> EnemyClass = GetBestEnemyClass(SpawnInfo.SpawnProbs);
		FTransform WorldSpawnTransform = FTransform::Identity;

		if (EnemyClass && TakeBestWorldSpawnTransform(SpawnInfo, WorldSpawnTransform))
		{
			FActorSpawnParameters SpawnParam;
			SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			// PatorlActor를 사용하도록 설정
			AEnemyCharacter* EnemyCharacter = GetWorld()->SpawnActorDeferred<AEnemyCharacter>(EnemyClass, WorldSpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
			if (SpawnInfo.PatrolPath)
			{
				EnemyCharacter->SetPatrolPathActor(SpawnInfo.PatrolPath);
			}

			// 정보 추적을 위해 ActiveInfo에 추가
			FActiveAIInfo& NewEntry = ActiveInfos.AddDefaulted_GetRef();
			NewEntry.EnemyInstance = EnemyCharacter;
			NewEntry.EnemyClass = EnemyClass;
			NewEntry.SpawnedLocation = WorldSpawnTransform.GetLocation();
			NewEntry.bIsAlive = true;

			// Finish & Notify
			EnemyCharacter->OnDead.AddDynamic(this, &AAISpawner::OnEnemyDead);
			EnemyCharacter->FinishSpawning(WorldSpawnTransform, true);
			EnemyCharacter->InitializeFromSpawner();
		}
	}
}

bool AAISpawner::TakeBestWorldSpawnTransform(FAISpawnInfo& InSpawnInfo, FTransform& OutWorldTransform) const
{
	bool bSuccess = InSpawnInfo.TakeSpawnTransform(OutWorldTransform);
	OutWorldTransform = OutWorldTransform * GetActorTransform();
	return bSuccess;
}

TSubclassOf<AEnemyCharacter> AAISpawner::GetBestEnemyClass(const TArray<FAISpawnProb>& InSpawnProbs) const
{
	TSubclassOf<AEnemyCharacter> Result = nullptr;

	int32 TotalProb = GetTotalProb(InSpawnProbs);
	int32 RandValue = FMath::RandRange(1, TotalProb);
	int32 CurrProb = 0;

	for (int32 i = 0; i < InSpawnProbs.Num(); i++)
	{
		CurrProb += InSpawnProbs[i].SpawnProb;
		if (RandValue <= CurrProb)
		{
			TSoftClassPtr<AEnemyCharacter> EnemyClass = InSpawnProbs[i].EnemyClass;
			Result = EnemyClass.LoadSynchronous();
			break;
		}
	}
	return Result;
}

int32 AAISpawner::GetTotalProb(const TArray<FAISpawnProb>& InProbs) const
{
	int32 TotalProb = 0;
	for (const FAISpawnProb& Elem : InProbs)
	{
		TotalProb += Elem.SpawnProb;
	}
	return TotalProb;
}

void AAISpawner::OnEnemyDead(ACombatCharacter* Victim)
{
	for (FActiveAIInfo& ActiveInfo : ActiveInfos)
	{
		if (ActiveInfo.EnemyInstance == Victim)
		{
			ActiveInfo.bIsAlive = false;
			ActiveInfo.EnemyInstance->OnDead.RemoveDynamic(this, &AAISpawner::OnEnemyDead);
		}
	}
}

bool AAISpawner::CanSpawnEnemy() const
{
	const UCombatDeveloperSettings* DeveloerSettings = GetDefault<UCombatDeveloperSettings>();
	if (DeveloerSettings->bDisableEnemySpawn)
	{
		return false;
	}

	return true;
}


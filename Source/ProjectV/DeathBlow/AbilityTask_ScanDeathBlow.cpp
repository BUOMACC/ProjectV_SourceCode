// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTask_ScanDeathBlow.h"
#include "AbilitySystemComponent.h"
#include "CombatGameplayTags.h"
#include "DeathBlowTargetInterface.h"
#include "FunctionLibrary/CommonFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"

UAbilityTask_ScanDeathBlow::UAbilityTask_ScanDeathBlow()
{
	bTickingTask = false;
	bSimulatedTask = false;
}

UAbilityTask_ScanDeathBlow* UAbilityTask_ScanDeathBlow::ScanDeathBlow(UGameplayAbility* OwningAbility, float InDeathBlowScanRate, float InDeathBlowScanRadius, TEnumAsByte<ETraceTypeQuery> InTraceChannel)
{
	UAbilityTask_ScanDeathBlow* MyTask = NewAbilityTask<UAbilityTask_ScanDeathBlow>(OwningAbility);
	MyTask->DeathBlowScanRate = InDeathBlowScanRate;
	MyTask->DeathBlowScanRadius = InDeathBlowScanRadius;
	MyTask->CollisionChannel = UEngineTypes::ConvertToCollisionChannel(InTraceChannel);
	return MyTask;
}

void UAbilityTask_ScanDeathBlow::Activate()
{
	Super::Activate();

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(TimerHandle, this, &UAbilityTask_ScanDeathBlow::QueryDeathBlowActors, DeathBlowScanRate, true);
	}
}

void UAbilityTask_ScanDeathBlow::OnDestroy(bool AbilityIsEnding)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
	}

	Super::OnDestroy(AbilityIsEnding);
}

void UAbilityTask_ScanDeathBlow::QueryDeathBlowActors()
{
	UWorld* World = GetWorld();
	AActor* Actor = GetAvatarActor();

	if (World && Actor)
	{
		TSubclassOf<UDeathBlowExecutorAbility> AbilityToGrant;
		AActor* TargetSourceActor = nullptr;

		FCollisionQueryParams Param;
		Param.AddIgnoredActor(Actor);
		Param.bTraceComplex = false;

		// 오버랩된 대상을 타겟으로 한 인살 어빌리티 새로 부여 & 회수 대상 어빌리티에서 제외처리
		TArray<FOverlapResult> SphereTraceResults;
		World->OverlapMultiByChannel(SphereTraceResults, Actor->GetActorLocation(), FQuat::Identity, CollisionChannel, FCollisionShape::MakeSphere(DeathBlowScanRadius), Param);
		SphereTraceResults.Sort([&](const FOverlapResult& Lhs, const FOverlapResult& Rhs)
			{
				float Dist_Lhs = FVector::Distance(Lhs.GetActor()->GetActorLocation(), Actor->GetActorLocation());
				float Dist_Rhs = FVector::Distance(Rhs.GetActor()->GetActorLocation(), Actor->GetActorLocation());
				return Dist_Lhs <= Dist_Rhs;
			});

		if (SphereTraceResults.Num() > 0)
		{
			for (const FOverlapResult& Result : SphereTraceResults)
			{
				TScriptInterface<IDeathBlowTargetInterface> DeathBlowTargetActor(Result.GetActor());
				if (!DeathBlowTargetActor)
				{
					continue;
				}

				AbilityToGrant = IDeathBlowTargetInterface::Execute_GetDeathBlowExecutorAbility(DeathBlowTargetActor.GetObject(), Actor);
				TargetSourceActor = Result.GetActor();
				if (AbilityToGrant)
				{
					break;
				}
			}
		}

		bool bIsSameAbility = false;
		if (FGameplayAbilitySpec* GrantedSpec = AbilitySystemComponent->FindAbilitySpecFromHandle(GrantedAbilityHandle))
		{
			if (AbilityToGrant == GrantedSpec->Ability->GetClass() && TargetSourceActor == GrantedSpec->SourceObject)
			{
				bIsSameAbility = true;
			}
		}

		if (!bIsSameAbility)
		{
			if (FGameplayAbilitySpec* GrantedSpec = AbilitySystemComponent->FindAbilitySpecFromHandle(GrantedAbilityHandle))
			{
				if (GrantedSpec->IsActive() == false)
				{
					AbilitySystemComponent->ClearAbility(GrantedAbilityHandle);
					GrantedAbilityHandle = FGameplayAbilitySpecHandle();
					UE_LOG(LogTemp, Warning, TEXT("UAbilityTask_ScanDeathBlow::QueryDeathBlowActors - Ability Cleared. (ability is changed.)"));
				}
			}

			if (AbilityToGrant)
			{
				FGameplayAbilitySpec Spec(AbilityToGrant, 1, INDEX_NONE, TargetSourceActor);
				GrantedAbilityHandle = AbilitySystemComponent->GiveAbility(Spec);
				UE_LOG(LogTemp, Warning, TEXT("UAbilityTask_ScanDeathBlow::QueryDeathBlowActors - Ability Applied."));
			}
		}
	}
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilityTask_WaitVelocityBelowThreshold.h"
#include "GameFramework/MovementComponent.h"

UAbilityTask_WaitVelocityBelowThreshold::UAbilityTask_WaitVelocityBelowThreshold(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bTickingTask = true;
}

void UAbilityTask_WaitVelocityBelowThreshold::Activate()
{
	const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
	CachedMovementComponent = ActorInfo->MovementComponent.Get();
	SetWaitingOnAvatar();
}

void UAbilityTask_WaitVelocityBelowThreshold::TickTask(float DeltaTime)
{
	if (CachedMovementComponent)
	{
		FVector Velocity = CachedMovementComponent->Velocity;
		float VelocityMagnitude = (bIgnoreZ) ? Velocity.Size2D() : Velocity.Size();

		if (VelocityMagnitude <= VelocityThreshold)
		{
			if (ShouldBroadcastAbilityTaskDelegates())
			{
				OnVelocityBelowThreshold.Broadcast();
			}
			EndTask();
		}
	}
	else
	{
		EndTask();
	}
}

void UAbilityTask_WaitVelocityBelowThreshold::OnDestroy(bool AbilityIsEnding)
{
	Super::OnDestroy(AbilityIsEnding);
}

UAbilityTask_WaitVelocityBelowThreshold* UAbilityTask_WaitVelocityBelowThreshold::WaitVelocityBelowThreshold(UGameplayAbility* OwningAbility, bool bIgnoreZ, float InVelocityThreshold)
{
	UAbilityTask_WaitVelocityBelowThreshold* MyObj = NewAbilityTask<UAbilityTask_WaitVelocityBelowThreshold>(OwningAbility);

	MyObj->bIgnoreZ = bIgnoreZ;
	MyObj->VelocityThreshold = InVelocityThreshold;

	return MyObj;
}

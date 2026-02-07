// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilityTask_WaitStartAccelerate.h"
#include "GameFramework/CharacterMovementComponent.h"

UAbilityTask_WaitStartAccelerate::UAbilityTask_WaitStartAccelerate(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bTickingTask = true;
}

void UAbilityTask_WaitStartAccelerate::Activate()
{
	const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
	CachedMovementComponent = Cast<UCharacterMovementComponent>(ActorInfo->MovementComponent.Get());
	SetWaitingOnAvatar();

	if (bCheckOnActivate)
	{
		if (CachedMovementComponent->GetCurrentAcceleration().Size() > 0.f)
		{
			if (ShouldBroadcastAbilityTaskDelegates())
			{
				OnStartAccelerate.Broadcast();
			}
			EndTask();
		}
	}
}

void UAbilityTask_WaitStartAccelerate::TickTask(float DeltaTime)
{
	if (CachedMovementComponent)
	{
		if (CachedMovementComponent->GetCurrentAcceleration().Size() > 0.f)
		{
			if (ShouldBroadcastAbilityTaskDelegates())
			{
				OnStartAccelerate.Broadcast();
			}
			EndTask();
		}
	}
	else
	{
		EndTask();
	}
}

void UAbilityTask_WaitStartAccelerate::OnDestroy(bool AbilityIsEnding)
{
	Super::OnDestroy(AbilityIsEnding);
}

UAbilityTask_WaitStartAccelerate* UAbilityTask_WaitStartAccelerate::WaitStartAccelerate(UGameplayAbility* OwningAbility, bool bCheckOnActivate)
{
	UAbilityTask_WaitStartAccelerate* MyObj = NewAbilityTask<UAbilityTask_WaitStartAccelerate>(OwningAbility);

	MyObj->bCheckOnActivate = bCheckOnActivate;

	return MyObj;
}

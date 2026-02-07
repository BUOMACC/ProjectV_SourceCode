// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTask_Tick.h"
#include "AbilitySystemComponent.h"
#include "FunctionLibrary/CommonFunctionLibrary.h"
#include "Interface/InteractionInterface.h"
#include "Kismet/KismetMathLibrary.h"

UAbilityTask_Tick::UAbilityTask_Tick()
{
	bTickingTask = true;
}

UAbilityTask_Tick* UAbilityTask_Tick::Tick(UGameplayAbility* OwningAbility)
{
	UAbilityTask_Tick* MyTask = NewAbilityTask<UAbilityTask_Tick>(OwningAbility);
	return MyTask;
}

void UAbilityTask_Tick::Activate()
{
	Super::Activate();
}

void UAbilityTask_Tick::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnTick.Broadcast(DeltaTime);
	}
}

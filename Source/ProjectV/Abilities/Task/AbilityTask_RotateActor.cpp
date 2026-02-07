// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTask_RotateActor.h"
#include "AbilitySystemComponent.h"
#include "CombatGameplayTags.h"
#include "CombatMovementComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/KismetMathLibrary.h"

UAbilityTask_RotateActor::UAbilityTask_RotateActor()
{
	bTickingTask = true;
	bSimulatedTask = false;
	bIsFinished = false;

	RotateTaskRule = ERotateTaskRule::None;
	RotateSpeed = 0.f;
	Duration = 0.f;
}

UAbilityTask_RotateActor* UAbilityTask_RotateActor::RotateActor(UGameplayAbility* OwningAbility, FName TaskInstanceName, ERotateTaskRule RotateTaskRule, float RotateSpeed, float Duration, bool bIgnoreLockon)
{
	UAbilityTask_RotateActor* MyTask = NewAbilityTask<UAbilityTask_RotateActor>(OwningAbility, TaskInstanceName);

	MyTask->RotateTaskRule = RotateTaskRule;
	MyTask->RotateSpeed = RotateSpeed;
	MyTask->Duration = Duration;
	MyTask->bIgnoreLockon = bIgnoreLockon;
	return MyTask;
}

void UAbilityTask_RotateActor::Activate()
{
	Super::Activate();

	// Rule이 None? -> 즉시 종료
	if (RotateTaskRule == ERotateTaskRule::None)
	{
		bIsFinished = true;
		OnFinish.Broadcast();
		EndTask();
		return;
	}

	if (ACharacter* AvatarCharacter = Cast<ACharacter>(GetAvatarActor()))
	{
		StartRot = AvatarCharacter->GetActorRotation();
		GetTargetRotationByRotateTaskRule(RotateTaskRule, AvatarCharacter, TargetRot);

		CachedCombatMovementComponent = Cast<UCombatMovementComponent>(AvatarCharacter->GetCharacterMovement());
	}

	// Lockon 상태에서 IgnoreLockon이 아닌경우에만, 고정적으로 카메라로 회전하도록 한다.
	const FCombatGameplayTags& CombatTags = FCombatGameplayTags::Get();
	if (AbilitySystemComponent->HasMatchingGameplayTag(CombatTags.Character_State_Lockon)
		&& bIgnoreLockon == false)
	{
		RotateTaskRule = ERotateTaskRule::TrackCamera;
	}

	if (CachedCombatMovementComponent)
	{
		CachedCombatMovementComponent->StartRootMotionRotation(TargetRot, RotateSpeed);
	}
	
	RemainDuration = Duration;
}

void UAbilityTask_RotateActor::TickTask(float DeltaTime)
{
	if (bIsFinished || (GetAvatarActor()->GetLocalRole() <= ENetRole::ROLE_SimulatedProxy))
	{
		return;
	}

	Super::TickTask(DeltaTime);

	RemainDuration -= DeltaTime;

	if (ACharacter* AvatarCharacter = Cast<ACharacter>(GetAvatarActor()))
	{
		FRotator LerpRot = GetAvatarActor()->GetActorRotation();
		switch (RotateTaskRule)
		{
		case ERotateTaskRule::None:
		case ERotateTaskRule::InputVector:
		case ERotateTaskRule::Camera:
			break;
		case ERotateTaskRule::TrackCamera:
			GetTargetRotationByRotateTaskRule(RotateTaskRule, AvatarCharacter, TargetRot);
			if (CachedCombatMovementComponent)
			{
				CachedCombatMovementComponent->SetRootMotionRotation(TargetRot);
			}
			break;
		}
	}


	if (RemainDuration <= 0.f)
	{
		if (CachedCombatMovementComponent)
		{
			CachedCombatMovementComponent->EndRootMotionRotation();
		}

		RemainDuration = 0.f;
		bIsFinished = true;
		OnFinish.Broadcast();
		EndTask();
		return;
	}
}

void UAbilityTask_RotateActor::OnDestroy(bool AbilityIsEnding)
{
	CachedCombatMovementComponent = nullptr;
	Super::OnDestroy(AbilityIsEnding);
}

void UAbilityTask_RotateActor::GetTargetRotationByRotateTaskRule(ERotateTaskRule Rule, ACharacter* Character, FRotator& OutTargetRotation)
{
	if (Character == nullptr)
		return;

	OutTargetRotation = Character->GetActorRotation();

	switch (Rule)
	{
	case ERotateTaskRule::Camera:
	case ERotateTaskRule::TrackCamera:
		OutTargetRotation.Yaw = Character->GetControlRotation().Yaw;
		break;

	case ERotateTaskRule::InputVector:
		FVector Acceleration = Character->GetCharacterMovement()->GetCurrentAcceleration();
		if (Acceleration != FVector::ZeroVector)
		{
			OutTargetRotation.Yaw = UKismetMathLibrary::MakeRotFromX(Acceleration).Yaw;
		}
		break;
	}
}
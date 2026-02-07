// Fill out your copyright notice in the Description page of Project Settings.


#include "LockonAbility.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Task/AbilityTask_Tick.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/WidgetComponent.h"
#include "Development/CombatDeveloperSettings.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectVGameState.h"

ULockonAbility::ULockonAbility()
{
	// Tags
	AbilityTags.AddTag(FCombatGameplayTags::Get().Ability_Type_Lockon);

	bAutoActivated = true;

	// Default Value
	Range = 5000.f;
	SphereRadius = 300.f;
	TraceChannel = ETraceTypeQuery::TraceTypeQuery1;
	bShowDebug = false;

	// Inputs
	InputTag = FCombatGameplayTags::Get().Input_Type_Lockon;
	bAutoTriggerWhenInput = false;

	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
}

void ULockonAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_Tick* WaitInputTask = UAbilityTask_Tick::Tick(this);
	WaitInputTask->OnTick.AddDynamic(this, &ThisClass::OnTick);
	WaitInputTask->ReadyForActivation();

	WaitInputPress();
	WaitLockonChange();
}

bool ULockonAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void ULockonAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ClearTarget();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULockonAbility::OnTick(float DeltaTime)
{
	// Lockon - Controller Rotation
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		const FCombatGameplayTags& CombatTags = FCombatGameplayTags::Get();

		APlayerController* PC = Cast<APlayerController>(GetActorController());
		if (!IsValid(PC))
			return;

		if (TargetActor.IsValid())
		{
			bool bImplementLockonInterface = TargetActor->GetClass()->ImplementsInterface(ULockonInterface::StaticClass());
			if (bImplementLockonInterface && ASC->HasMatchingGameplayTag(CombatTags.Character_State_Lockon))
			{
				// Control Rotation
				if (IsLocallyControlled())
				{
					FRotator LookRotation = FRotator::ZeroRotator;

					FVector PawnLocation = GetCombatCharacter()->GetActorLocation();
					FVector TargetLocation = TargetActor->GetActorLocation();

					FVector PawnToTarget = (TargetLocation - PawnLocation);
					float PawnToTargetDist = PawnToTarget.Size2D();

					FVector StartLocation = PC->PlayerCameraManager->GetCameraLocation();
					
					// 거리에 따른 카메라 높이를 보정합니다. (가까움 -> 덜 내려다봄,  멂 -> 많이 내려다봄)
					float AdditionalZ = FMath::GetMappedRangeValueClamped(FVector2D(0.f, 1000.f), FVector2D(100.f, 400.f), PawnToTargetDist);
					StartLocation.Z += AdditionalZ;

					// 타겟보다 조금 더 떨어진 위치를 바라보도록 하여 너무 내려다보지 않도록 합니다.
					FVector LookLocation = TargetLocation + (PawnToTarget.GetSafeNormal2D() * 300.f);


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
					if (bShowDebug)
					{
						DrawDebugSphere(GetWorld(), LookLocation, 16.f, 12, FColor::Green, false, 0.016f);
						DrawDebugLine(GetWorld(), StartLocation, TargetLocation, FColor::Green, false, 0.016f);
					}
#endif
					PC->SetControlRotation(UKismetMathLibrary::FindLookAtRotation(StartLocation, LookLocation));
				}
			}
		}
	}

	if (IsLocallyControlled())
	{
		if (!TargetActor.IsValid() || !CanLockon(TargetActor.Get()))
		{
			ClearTarget();
		}
	}
}

void ULockonAbility::WaitInputPress()
{
	if (IsLocallyControlled())
	{
		UAbilityTask_WaitInputPress* WaitInputTask = UAbilityTask_WaitInputPress::WaitInputPress(this, false);
		WaitInputTask->OnPress.AddDynamic(this, &ThisClass::OnInputPress);
		WaitInputTask->ReadyForActivation();
	}
}

void ULockonAbility::WaitLockonChange()
{
	if (IsLocallyControlled())
	{
		UAbilityTask_WaitGameplayEvent* WaitEventTask_ChangeLeft = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FCombatGameplayTags::Get().Input_Type_LockonChange_Left, nullptr, false, true);
		WaitEventTask_ChangeLeft->EventReceived.AddDynamic(this, &ThisClass::OnReceiveLockonChangeLeft);
		WaitEventTask_ChangeLeft->ReadyForActivation();

		UAbilityTask_WaitGameplayEvent* WaitEventTask_ChangeRight = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FCombatGameplayTags::Get().Input_Type_LockonChange_Right, nullptr, false, true);
		WaitEventTask_ChangeRight->EventReceived.AddDynamic(this, &ThisClass::OnReceiveLockonChangeRight);
		WaitEventTask_ChangeRight->ReadyForActivation();
	}
}

void ULockonAbility::OnReceiveLockonChangeLeft(FGameplayEventData Payload)
{
	if (!TargetActor.IsValid())
		return;

	float AxisValue = Payload.EventMagnitude;
	if (bool bIsLeft = (AxisValue < 0.f))
	{
		TargetChangeLockonTrace(GetWorld(), -25.f);
	}
}

void ULockonAbility::OnReceiveLockonChangeRight(FGameplayEventData Payload)
{
	if (!TargetActor.IsValid())
		return;

	float AxisValue = Payload.EventMagnitude;
	if (bool bIsRight = (AxisValue > 0.f))
	{
		TargetChangeLockonTrace(GetWorld(), 25.f);
	}
}

void ULockonAbility::OnInputPress(float TimeWaited)
{
	if (!TargetActor.IsValid())
	{
		LockonTrace(GetWorld());
	}
	else
	{
		ClearTarget();
	}

	WaitInputPress();
}

void ULockonAbility::LockonTrace(UWorld* World)
{
	if (World == nullptr)
		return;

	if (IsLocallyControlled())
	{
		if (APlayerController* PC = Cast<APlayerController>(GetActorController()))
		{
			FVector StartLocation = PC->PlayerCameraManager->GetCameraLocation();
			FVector CameraForward = PC->GetControlRotation().Vector();
			FVector EndLocation = StartLocation + (CameraForward * Range);

			// 1) LineTrace -> 적확히 적중한 대상을 락온 우선순위로
			FHitResult LineTraceResult;
			TArray<AActor*> ActorsToIgnore = { GetCombatCharacter() };
			EDrawDebugTrace::Type DrawDebugType = (bShowDebug) ? EDrawDebugTrace::Type::ForDuration : EDrawDebugTrace::Type::None;

			// 1) 에임에 적중한 대상을 제일 우선순위로
			UKismetSystemLibrary::LineTraceSingle(World, StartLocation, EndLocation, TraceChannel, false, ActorsToIgnore, DrawDebugType, LineTraceResult, true, FLinearColor::Yellow);
			if (LineTraceResult.GetActor() && CanLockon(LineTraceResult.GetActor()))
			{
				SetLockonTarget(TargetActor.Get());
				return;
			}

			// 2) 주변 범위에 있는 대상을 두번째 우선순위로
			TArray<FHitResult> SphereTraceResults;
			UKismetSystemLibrary::SphereTraceMulti(World, StartLocation, EndLocation - (CameraForward * SphereRadius), SphereRadius, TraceChannel, false, ActorsToIgnore, DrawDebugType, SphereTraceResults, true);
			if (SphereTraceResults.Num() > 0)
			{
				AActor* BestActor = FindBestActor(SphereTraceResults);
				if (IsValid(BestActor) && CanLockon(BestActor))
				{
					SetLockonTarget(BestActor);
					return;
				}
			}
		}
	}
}

void ULockonAbility::TargetChangeLockonTrace(UWorld* World, float TraceAngle)
{
	if (World == nullptr)
		return;

	if (IsLocallyControlled())
	{
		if (APlayerController* PC = Cast<APlayerController>(GetActorController()))
		{
			FVector StartLocation = PC->PlayerCameraManager->GetCameraLocation();
			FVector CameraForward = PC->GetControlRotation().Vector();
			FVector RotateAxis = FVector::UpVector;
			CameraForward = CameraForward.RotateAngleAxis(TraceAngle, RotateAxis);
			FVector EndLocation = StartLocation + (CameraForward * Range);

			TArray<AActor*> ActorsToIgnore = { GetCombatCharacter() };
			EDrawDebugTrace::Type DrawDebugType = (bShowDebug) ? EDrawDebugTrace::Type::ForDuration : EDrawDebugTrace::Type::None;

			TArray<FHitResult> SphereTraceResults;
			UKismetSystemLibrary::SphereTraceMulti(World, StartLocation, EndLocation - (CameraForward * SphereRadius), SphereRadius, TraceChannel, false, ActorsToIgnore, DrawDebugType, SphereTraceResults, true);
			if (SphereTraceResults.Num() > 0)
			{
				AActor* BestActor = FindBestNearbyActor(SphereTraceResults, TargetActor.Get(), ActorsToIgnore, (TraceAngle < 0.f));
				if (IsValid(BestActor) && CanLockon(BestActor))
				{
					ClearTarget();
					SetLockonTarget(BestActor);
					return;
				}
			}
		}
	}
}

void ULockonAbility::SetLockonTarget(AActor* InActor)
{
	if (!IsValid(InActor))
		return;

	if (TargetActor != InActor)
	{
		AActor* PrevTarget = (TargetActor.IsValid()) ? TargetActor.Get() : nullptr;

		SetLockonWidgetVisibility(PrevTarget, false);

		TargetActor = InActor;

		if (AController* Controller = GetActorController())
		{
			Controller->SetIgnoreLookInput(true);
		}

		SetLockonWidgetVisibility(InActor, true);

		GetCharacterMovementComponent()->bUseControllerDesiredRotation = true;
		GetCharacterMovementComponent()->bOrientRotationToMovement = false;

		GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(FCombatGameplayTags::Get().Character_State_Lockon);
		GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(FCombatGameplayTags::Get().Character_State_CombatMode);

		if (HasAuthority(&CurrentActivationInfo) && IsLocallyControlled())
		{
			GetAbilitySystemComponentFromActorInfo()->AddReplicatedLooseGameplayTag(FCombatGameplayTags::Get().Character_State_Lockon);
			GetAbilitySystemComponentFromActorInfo()->AddReplicatedLooseGameplayTag(FCombatGameplayTags::Get().Character_State_CombatMode);
		}

		if (IsPredictingClient())
		{
			SetLockonTarget_Server(InActor);
		}
	}
}

void ULockonAbility::SetLockonTarget_Server_Implementation(AActor* InActor)
{
	TargetActor = InActor;

	GetCharacterMovementComponent()->bUseControllerDesiredRotation = true;
	GetCharacterMovementComponent()->bOrientRotationToMovement = false;

	GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(FCombatGameplayTags::Get().Character_State_Lockon);
	GetAbilitySystemComponentFromActorInfo()->AddReplicatedLooseGameplayTag(FCombatGameplayTags::Get().Character_State_Lockon);

	GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(FCombatGameplayTags::Get().Character_State_CombatMode);
	GetAbilitySystemComponentFromActorInfo()->AddReplicatedLooseGameplayTag(FCombatGameplayTags::Get().Character_State_CombatMode);
}

void ULockonAbility::ClearTarget()
{
	if (TargetActor.IsValid())
	{
		SetLockonWidgetVisibility(TargetActor.Get(), false);
		TargetActor = nullptr;

		if (AController* Controller = GetActorController())
		{
			/* SetIgnoreInput(false)이 카운팅으로 관리되므로, 타겟 변경시 IgnoreInput 카운팅이 누적되어 리셋시켜줘야 함 */
			Controller->ResetIgnoreLookInput();
		}

		GetCharacterMovementComponent()->bUseControllerDesiredRotation = false;
		GetCharacterMovementComponent()->bOrientRotationToMovement = true;

		GetAbilitySystemComponentFromActorInfo()->SetLooseGameplayTagCount(FCombatGameplayTags::Get().Character_State_Lockon, 0);
		GetAbilitySystemComponentFromActorInfo()->SetLooseGameplayTagCount(FCombatGameplayTags::Get().Character_State_CombatMode, 0);

		if (IsPredictingClient())
		{
			ClearTarget_Server();
		}
	}
}

void ULockonAbility::ClearTarget_Server_Implementation()
{
	if (TargetActor.IsValid())
	{
		TargetActor = nullptr;
	}

	GetCharacterMovementComponent()->bUseControllerDesiredRotation = false;
	GetCharacterMovementComponent()->bOrientRotationToMovement = true;

	GetAbilitySystemComponentFromActorInfo()->SetLooseGameplayTagCount(FCombatGameplayTags::Get().Character_State_Lockon, 0);
	GetAbilitySystemComponentFromActorInfo()->SetReplicatedLooseGameplayTagCount(FCombatGameplayTags::Get().Character_State_Lockon, 0);

	GetAbilitySystemComponentFromActorInfo()->SetLooseGameplayTagCount(FCombatGameplayTags::Get().Character_State_CombatMode, 0);
	GetAbilitySystemComponentFromActorInfo()->SetReplicatedLooseGameplayTagCount(FCombatGameplayTags::Get().Character_State_CombatMode, 0);
}

AActor* ULockonAbility::FindBestActor(const TArray<FHitResult>& HitResults)
{
	AActor* BestActor = nullptr;
	double BestDistance = (Range * 1.2f); // 최소값 선별 전 기본값을 충분히 높은 값으로 설정 (* 1.2는 여유값)

	if (APlayerController* PC = Cast<APlayerController>(GetActorController()))
	{
		int32 Viewport_X, Viewport_Y = 0;
		PC->GetViewportSize(Viewport_X, Viewport_Y);

		FVector2D ScreenCenterPos = FVector2D(Viewport_X / 2, Viewport_Y / 2);

		for (const FHitResult& Element : HitResults)
		{
			AActor* CurrentActor = Element.GetActor();
			if (IsValid(CurrentActor) == false)
				continue;

			FVector2D ScreenPos = FVector2D::ZeroVector;
			PC->ProjectWorldLocationToScreen(CurrentActor->GetActorLocation(), ScreenPos);

		 	double Dist = FVector2D::Distance(ScreenCenterPos, ScreenPos);

			if (Dist < BestDistance)
			{
				BestActor = CurrentActor;
				BestDistance = Dist;
			}
		}
	}
	return BestActor;
}

AActor* ULockonAbility::FindBestNearbyActor(const TArray<FHitResult>& HitResults, AActor* Pivot, TArray<AActor*> ActorToIgnore, bool bIsLeft)
{
	if (IsValid(Pivot) == false)
		return nullptr;

	AActor* BestActor = nullptr;
	double BestDistance = (Range * 1.2f); // 최소값 선별 전 기본값을 충분히 높은 값으로 설정 (* 1.2는 여유값)

	if (APlayerController* PC = Cast<APlayerController>(GetActorController()))
	{
		FVector2D PivotPos = FVector2D::ZeroVector;
		PC->ProjectWorldLocationToScreen(Pivot->GetActorLocation(), PivotPos);

		for (const FHitResult& Element : HitResults)
		{
			AActor* CurrentActor = Element.GetActor();
			if (IsValid(CurrentActor) == false || ActorToIgnore.Contains(CurrentActor))
				continue;

			// 카메라 기준 앞에있는 대상만 판별
			FVector CameraLocation = PC->PlayerCameraManager->GetCameraLocation();

			FRotator CameraRotation = PC->PlayerCameraManager->GetCameraRotation();
			CameraRotation.Pitch = 0.f;
			CameraRotation.Roll = 0.f;

			FVector CameraToTargetDir = (CurrentActor->GetActorLocation() - CameraLocation).GetSafeNormal();
			FVector CameraYawDir = CameraRotation.Vector();
			if (CameraYawDir.Dot(CameraToTargetDir) <= 0.f)
				continue;

			FVector2D ScreenPos = FVector2D::ZeroVector;
			PC->ProjectWorldLocationToScreen(CurrentActor->GetActorLocation(), ScreenPos);

			// 왼쪽 또는 오른쪽 대상인지 판별
			float DiffX = ScreenPos.X - PivotPos.X;
			if (bIsLeft && DiffX >= 0) continue;
			if (!bIsLeft && DiffX <= 0) continue;

			double Dist = FVector2D::Distance(PivotPos, ScreenPos);

			if (Dist < BestDistance)
			{
				BestActor = CurrentActor;
				BestDistance = Dist;
			}
		}
	}
	return BestActor;
}

void ULockonAbility::SetLockonWidgetVisibility(AActor* InActor, bool bIsVisible)
{
	if (!IsValid(InActor))
		return;

	if (InActor->GetClass()->ImplementsInterface(ULockonInterface::StaticClass()))
	{
		if (UWidgetComponent* LockonWidget = ILockonInterface::Execute_GetLockonWidget(InActor))
		{
			LockonWidget->SetVisibility(bIsVisible);
			LockonWidget->UpdateWidget();
		}
	}
}

bool ULockonAbility::CanLockon(AActor* InActor) const
{
	if (!IsValid(InActor) || !IsValid(GetAvatarActorFromActorInfo()))
		return false;

	// Team?
	const AProjectVGameState* GameState = GetWorld()->GetGameState<AProjectVGameState>();
	ETeamAttitude::Type Attitude = FGenericTeamId::GetAttitude(GetAvatarActorFromActorInfo(), InActor);
	if (Attitude == ETeamAttitude::Friendly && !GameState->CanTeamKill())
	{
		return false;
	}

	// Dead?
	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor))
	{
		if (TargetASC->HasMatchingGameplayTag(FCombatGameplayTags::Get().Character_MainState_Dead))
		{
			return false;
		}
	}

	// Out of Range?
	if (APlayerController* PC = Cast<APlayerController>(GetActorController()))
	{
		FVector StartLocation = PC->PlayerCameraManager->GetCameraLocation();
		FVector EndLocation = InActor->GetActorLocation();

		FHitResult LineTraceResult;
		TArray<AActor*> ActorsToIgnore = { GetCombatCharacter() };

		bool bIsHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartLocation, EndLocation, WallCheckTraceChannel, false, ActorsToIgnore, EDrawDebugTrace::None, LineTraceResult, true);
		if (bIsHit)
		{
			return false;
		}
	}

	return true;
}
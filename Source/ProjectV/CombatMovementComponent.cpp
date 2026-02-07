// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatMovementComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "CombatGameplayTags.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "ProjectVLogs.h"

// Combat Network Move Data
void FCombatNetworkMoveData::ClientFillNetworkMoveData(const FSavedMove_Character& ClientMove, ENetworkMoveType MoveType)
{
	Super::ClientFillNetworkMoveData(ClientMove, MoveType);

	FSavedMove_Combat& CombatClientMove = static_cast<FSavedMove_Combat&>(const_cast<FSavedMove_Character&>(ClientMove));
	DesiredTargetRotation = CombatClientMove.DesiredTargetRotation;
	RotateSpeedPerSec = CombatClientMove.RotateSpeedPerSec;
}

bool FCombatNetworkMoveData::Serialize(UCharacterMovementComponent& CharacterMovement, FArchive& Ar, UPackageMap* PackageMap, ENetworkMoveType MoveType)
{
	bool bResult = Super::Serialize(CharacterMovement, Ar, PackageMap, MoveType);

	DesiredTargetRotation.NetSerialize(Ar, PackageMap, bResult);
	
	Ar << RotateSpeedPerSec;

	return bResult;
}

// Saved Move
void FSavedMove_Combat::Clear()
{
	Super::Clear();
	bWantsToRootMotionRotation = false;
	DesiredTargetRotation = FRotator::ZeroRotator;
	RotateSpeedPerSec = 0.f;
}

void FSavedMove_Combat::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	if (UCombatMovementComponent* CombatCMC = Cast<UCombatMovementComponent>(C->GetCharacterMovement()))
	{
		bWantsToRootMotionRotation = CombatCMC->bWantsToRootMotionRotation;
		DesiredTargetRotation = CombatCMC->DesiredTargetRotation;
		RotateSpeedPerSec = CombatCMC->RotateSpeedPerSec;
	}
}

void FSavedMove_Combat::PostUpdate(ACharacter* C, EPostUpdateMode PostUpdateMode)
{
	Super::PostUpdate(C, PostUpdateMode);
}

bool FSavedMove_Combat::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const
{
	bool bResult = Super::CanCombineWith(NewMove, InCharacter, MaxDelta);

	const FSavedMove_Combat* NewCombatMove = static_cast<FSavedMove_Combat*>(NewMove.Get());
	if (bWantsToRootMotionRotation != NewCombatMove->bWantsToRootMotionRotation)
	{
		return false;
	}

	const float AngleTolerance = 1e-3f;
	if (!FMath::IsNearlyEqual(RotateSpeedPerSec, NewCombatMove->RotateSpeedPerSec, AngleTolerance))
	{
		return false;
	}
	
	if (!DesiredTargetRotation.Equals(NewCombatMove->DesiredTargetRotation, AngleTolerance))
	{
		return false;
	}

	return bResult;
}

void FSavedMove_Combat::PrepMoveFor(ACharacter* C)
{
	Super::PrepMoveFor(C);

	if (UCombatMovementComponent* CombatCMC = Cast<UCombatMovementComponent>(C->GetCharacterMovement()))
	{
		bWantsToRootMotionRotation = CombatCMC->bWantsToRootMotionRotation;
		DesiredTargetRotation = CombatCMC->DesiredTargetRotation;
		RotateSpeedPerSec = CombatCMC->RotateSpeedPerSec;
	}
}

uint8 FSavedMove_Combat::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (bWantsToRootMotionRotation)
	{
		Result |= FLAG_Custom_0;
	}

	return Result;
}


// Network Prediction Data
FNetworkPredictionData_Client_Combat::FNetworkPredictionData_Client_Combat(const UCharacterMovementComponent& ClientMovement)
	: Super(ClientMovement)
{
}

FSavedMovePtr FNetworkPredictionData_Client_Combat::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_Combat());
}

// CMC
UCombatMovementComponent::UCombatMovementComponent()
	: bHasReplicatedAcceleration(false),
	  bWantsToRootMotionRotation(false),
	  DesiredTargetRotation(FRotator::ZeroRotator),
	  RotateSpeedPerSec(0.f)
{
	bAllowPhysicsRotationDuringAnimRootMotion = true;

	SetNetworkMoveDataContainer(CombatNetworkMoveDataContainer);
}

void UCombatMovementComponent::PhysicsRotation(float DeltaTime)
{
	// * 루트 모션 애니메이션이 실행중이 아닌경우 기존대로 처리
	if (!HasAnimRootMotion())
	{
		Super::PhysicsRotation(DeltaTime);
		return;
	}

	if (!bWantsToRootMotionRotation)
	{
		return;
	}

	// * 루트 모션인 경우, 설정된 목표 회전(DesiredTargetRotation)에 도달하도록 회전
	FRotator CurrentRotation = UpdatedComponent->GetComponentRotation();
	FRotator DeltaRotation = FRotator(0.f, RotateSpeedPerSec * DeltaTime, 0.f);
	FRotator DesiredRotation = DesiredTargetRotation;

	const float AngleTolerance = 1e-3f;
	if (!CurrentRotation.Equals(DesiredRotation, AngleTolerance))
	{
		if (!FMath::IsNearlyEqual(CurrentRotation.Pitch, DesiredRotation.Pitch, AngleTolerance))
		{
			DesiredRotation.Pitch = FMath::FixedTurn(CurrentRotation.Pitch, DesiredRotation.Pitch, DeltaRotation.Pitch);
		}

		if (!FMath::IsNearlyEqual(CurrentRotation.Yaw, DesiredRotation.Yaw, AngleTolerance))
		{
			DesiredRotation.Yaw = FMath::FixedTurn(CurrentRotation.Yaw, DesiredRotation.Yaw, DeltaRotation.Yaw);
		}

		if (!FMath::IsNearlyEqual(CurrentRotation.Roll, DesiredRotation.Roll, AngleTolerance))
		{
			DesiredRotation.Roll = FMath::FixedTurn(CurrentRotation.Roll, DesiredRotation.Roll, DeltaRotation.Roll);
		}
	}

	MoveUpdatedComponent(FVector::ZeroVector, DesiredRotation, /*bSweep*/ false);
}

FNetworkPredictionData_Client* UCombatMovementComponent::GetPredictionData_Client() const
{
	if (ClientPredictionData == nullptr)
	{
		UCombatMovementComponent* MutableThis = const_cast<UCombatMovementComponent*>(this);
		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_Combat(*this);
	}

	return ClientPredictionData;
}

void UCombatMovementComponent::SimulateMovement(float DeltaTime)
{
	if (bHasReplicatedAcceleration)
	{
		// Preserve our replicated acceleration (복제된 Acceleration으로 덮어씌움)
		const FVector OriginalAcceleration = Acceleration;
		Super::SimulateMovement(DeltaTime);
		Acceleration = OriginalAcceleration;
	}
	else
	{
		Super::SimulateMovement(DeltaTime);
	}
}

float UCombatMovementComponent::GetMaxSpeed() const
{
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()))
	{
		const FGameplayTag& BlockMovementTag = FCombatGameplayTags::Get().Character_State_BlockMovement;
		if (ASC->HasMatchingGameplayTag(BlockMovementTag))
		{
			return 0.f;
		}
	}

	return Super::GetMaxSpeed();
}

FRotator UCombatMovementComponent::GetDeltaRotation(float DeltaTime) const
{
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()))
	{
		const FGameplayTag& BlockMovementTag = FCombatGameplayTags::Get().Character_State_BlockMovement;
		if (ASC->HasMatchingGameplayTag(BlockMovementTag))
		{
			return FRotator::ZeroRotator;
		}
	}

	return Super::GetDeltaRotation(DeltaTime);
}

void UCombatMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	if (!CharacterOwner)
	{
		return;
	}

	bool bPrevWantsToRootMotionRotation = bWantsToRootMotionRotation;
	bWantsToRootMotionRotation = ((Flags & FSavedMove_Character::FLAG_Custom_0) != 0);

	bool bIsChanged = (bPrevWantsToRootMotionRotation != bWantsToRootMotionRotation);
	
	// Reset Data
	if (bIsChanged && !bWantsToRootMotionRotation)
	{
		DesiredTargetRotation = FRotator::ZeroRotator;
		RotateSpeedPerSec = 0.f;
	}
}

void UCombatMovementComponent::MoveAutonomous(float ClientTimeStamp, float DeltaTime, uint8 CompressedFlags, const FVector& NewAccel)
{
	// Client에서 받은 데이터를 기반으로 CMC 데이터 업데이트
	UpdateFromCustomDatas();

	Super::MoveAutonomous(ClientTimeStamp, DeltaTime, CompressedFlags, NewAccel);
}

void UCombatMovementComponent::UpdateFromCustomDatas()
{
	FCharacterNetworkMoveData* NetworkMoveData = GetCurrentNetworkMoveData();
	if (!NetworkMoveData)
	{
		return;
	}

	if (FCombatNetworkMoveData* CombatNetworkMoveData = static_cast<FCombatNetworkMoveData*>(NetworkMoveData))
	{
		DesiredTargetRotation = CombatNetworkMoveData->DesiredTargetRotation;
		RotateSpeedPerSec = CombatNetworkMoveData->RotateSpeedPerSec;
	}
}

void UCombatMovementComponent::SetReplicatedAcceleration(const FVector& InAcceleration)
{
	bHasReplicatedAcceleration = true;
	Acceleration = InAcceleration;
}

bool UCombatMovementComponent::CanUnCrouch() const
{
	if (!HasValidData() || !CharacterOwner || !CharacterOwner->GetCapsuleComponent())
	{
		return false;
	}

	UWorld* MyWorld = GetWorld();
	if (!MyWorld)
	{
		return false;
	}

	// Get Current Capsule Info
	UCapsuleComponent* CapsuleComp = CharacterOwner->GetCapsuleComponent();
	FVector CurrentShapeScale = CapsuleComp->GetComponentScale();

	// Get CDO Capsule Info
	ACharacter* CharacterCDO = CharacterOwner->GetClass()->GetDefaultObject<ACharacter>();
	float DefaultUnscaledRadius = CharacterCDO->GetCapsuleComponent()->GetUnscaledCapsuleRadius();
	float DefaultUnscaledHalfHeight = CharacterCDO->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();

	// CDO에 스케일을 적용했을때 크기 (월드에 배치한 이후 스케일을 변경할수 있으므로 스케일값은 현재 캡슐의 스케일을 사용)
	// Radius는 큰 값을 기준으로 스케일 적용 (GetScaledCapsuleRadius() 참고)
	float DefaultScaledRadius = DefaultUnscaledRadius * FMath::Max(CurrentShapeScale.X, CurrentShapeScale.Y);
	float DefaultScaledHalfHeight = CharacterCDO->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() * CurrentShapeScale.Z;

	// 캐릭터 CDO에서 지정한 캡슐값과 동일한 경우 -> 서있는 상태를 의미함
	if (FMath::IsNearlyEqual(CapsuleComp->GetUnscaledCapsuleHalfHeight(), DefaultUnscaledHalfHeight))
	{
		return true;
	}

	// 캡슐의 위치 (Capsule Center)
	FVector CapsuleLocation = UpdatedComponent->GetComponentLocation();

	FCollisionQueryParams CapsuleParams(SCENE_QUERY_STAT(CanUnCrouchTrace), false, CharacterOwner);
	FCollisionResponseParams ResponseParam;
	InitCollisionParams(CapsuleParams, ResponseParam);

	FCollisionShape StandingCapsuleShape = FCollisionShape::MakeCapsule(DefaultScaledRadius, DefaultScaledHalfHeight);
	ECollisionChannel CollisionChannel = UpdatedComponent->GetCollisionObjectType();

	/*
	* 서있을때 캡슐의 위치를 구합니다.
	* 앉아있는 경우 중앙좌표값은 서있을때보다 상대적으로 낮은 좌표를 반환하게 됩니다.
	* 앉은 상태의 캡슐높이만큼 상쇄하여 땅바닥 위치의 좌표를 구합니다.
	* 마지막으로 땅바닥 위치에 서있을때 캡슐 높이만큼 추가하여 캡슐의 실제 위치(중앙)를 구합니다.
	* 
	* 
	*   < 앉은경우 >			< 서있는 경우 >
	*							 ____
	*							/	 \
	*		____				|	 |
	*	   /    \				|  C |
	*	   |  C |				|	 |
	*	   \____/				\____/
	* 
	*/

	float StandingFootLocationZ = CapsuleLocation.Z - CrouchedHalfHeight;
	float StandingCenterLocationZ = StandingFootLocationZ + DefaultScaledHalfHeight;
	FVector StandingLocationCenter = FVector(CapsuleLocation.X, CapsuleLocation.Y, StandingCenterLocationZ);

	// DrawDebugCapsule(MyWorld, StandingLocationCenter, CharacterCDO->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight(), DefaultScaledRadius, FQuat::Identity, FColor::Cyan, false, 5.f);

	bool bOverlapped = MyWorld->OverlapBlockingTestByChannel(StandingLocationCenter, FQuat::Identity, CollisionChannel, StandingCapsuleShape, CapsuleParams, ResponseParam);
	return !bOverlapped;
}

void UCombatMovementComponent::StartRootMotionRotation(const FRotator& NewDesiredTargetRotation, float NewRotateSpeedPerSec)
{
	if (CharacterOwner->IsLocallyControlled())
	{
		DesiredTargetRotation = NewDesiredTargetRotation;
		DesiredTargetRotation.Normalize();

		RotateSpeedPerSec = FMath::Max(0.f, NewRotateSpeedPerSec);

		bWantsToRootMotionRotation = true;
	}
}

void UCombatMovementComponent::SetRootMotionRotation(const FRotator& NewDesiredTargetRotation)
{
	if (CharacterOwner->IsLocallyControlled())
	{
		if (!bAllowPhysicsRotationDuringAnimRootMotion)
			return;

		const float AngleTolerance = 1e-3f;
		if (!NewDesiredTargetRotation.Equals(DesiredTargetRotation, AngleTolerance))
		{
			DesiredTargetRotation = NewDesiredTargetRotation;
			DesiredTargetRotation.Normalize();
		}
	}
}

void UCombatMovementComponent::EndRootMotionRotation()
{
	if (CharacterOwner->IsLocallyControlled())
	{
		bWantsToRootMotionRotation = false;
	}
}
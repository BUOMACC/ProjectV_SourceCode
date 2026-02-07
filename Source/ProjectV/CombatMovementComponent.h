// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CombatMovementComponent.generated.h"

 // Network Move Data
struct PROJECTV_API FCombatNetworkMoveData : public FCharacterNetworkMoveData
{
	typedef FCharacterNetworkMoveData Super;

public:
	FCombatNetworkMoveData()
		: DesiredTargetRotation(FRotator::ZeroRotator),
		  RotateSpeedPerSec(0.f)
	{
	}

	virtual void ClientFillNetworkMoveData(const FSavedMove_Character& ClientMove, ENetworkMoveType MoveType) override;

	virtual bool Serialize(UCharacterMovementComponent& CharacterMovement, FArchive& Ar, UPackageMap* PackageMap, ENetworkMoveType MoveType) override;

public:
	FRotator DesiredTargetRotation;
	float RotateSpeedPerSec;
};

 // Network Move Data Container
struct PROJECTV_API FCombatNetworkMoveDataContainer : public FCharacterNetworkMoveDataContainer
{
public:
	FCombatNetworkMoveDataContainer()
	{
		NewMoveData		= &CombatDefaultMoveData[0];
		PendingMoveData = &CombatDefaultMoveData[1];
		OldMoveData		= &CombatDefaultMoveData[2];
	}

protected:
	FCombatNetworkMoveData CombatDefaultMoveData[3];
};


// Saved Move
class PROJECTV_API FSavedMove_Combat : public FSavedMove_Character
{
	typedef FSavedMove_Character Super;

public:
	virtual void Clear() override;
	virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData) override;
	virtual void PostUpdate(ACharacter* C, EPostUpdateMode PostUpdateMode) override;
	virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
	virtual void PrepMoveFor(ACharacter* C) override;
	virtual uint8 GetCompressedFlags() const override;

public:
	bool bWantsToRootMotionRotation;

	FRotator DesiredTargetRotation;
	float RotateSpeedPerSec;
};


// Network Prediction Data
class PROJECTV_API FNetworkPredictionData_Client_Combat : public FNetworkPredictionData_Client_Character
{
	typedef FNetworkPredictionData_Client_Character Super;

public:
	FNetworkPredictionData_Client_Combat(const UCharacterMovementComponent& ClientMovement);

	virtual FSavedMovePtr AllocateNewMove() override;
};


// CMC
UCLASS()
class PROJECTV_API UCombatMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
/*
	######################################
	#			  Functions				 #
	######################################
*/
public:
	UCombatMovementComponent();

public:
	virtual void PhysicsRotation(float DeltaTime) override;
	virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;

protected:
	virtual void SimulateMovement(float DeltaTime) override;
	virtual float GetMaxSpeed() const override;
	virtual FRotator GetDeltaRotation(float DeltaTime) const override;
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual void MoveAutonomous(float ClientTimeStamp, float DeltaTime, uint8 CompressedFlags, const FVector& NewAccel) override;

private:
	void UpdateFromCustomDatas();

public:
	void SetReplicatedAcceleration(const FVector& InAcceleration);

	/* 일어날 수 있는지 체크합니다. */
	bool CanUnCrouch() const;

public:
	/*
	* RootMotion에 동기화되는 회전을 적용합니다.
	* EndRootMotionRotation() 함수를 호출하여 중지할 수 있습니다.
	* 
	* @param NewDesiredTargetRotation	서서히 회전하여 도달해야할 목표 회전입니다.
	* @param NewRotateSpeedPerSec		초당 회전량 입니다.
	*/
	void StartRootMotionRotation(const FRotator& NewDesiredTargetRotation, float NewRotateSpeedPerSec);

	/* RootMotion에 동기화되는 회전 정보를 수정합니다. */
	void SetRootMotionRotation(const FRotator& NewDesiredTargetRotation);

	/* RootMotion에 동기화되는 회전을 중지합니다. */
	void EndRootMotionRotation();


/*
	######################################
	#			  Variables				 #
	######################################
*/
protected:
	UPROPERTY(Transient)
	bool bHasReplicatedAcceleration;

public:
	/* true인 경우, 루트모션 중 동기화 회전을 사용합니다. */
	bool bWantsToRootMotionRotation;

	/* 목표 회전 */
	FRotator DesiredTargetRotation;
	
	/* 초당 회전량 (0 ~ 360) */
	float RotateSpeedPerSec;

private:
	FCombatNetworkMoveDataContainer CombatNetworkMoveDataContainer;
};

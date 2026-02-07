// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_RootMotionScaler.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

UAnimNotifyState_RootMotionScaler::UAnimNotifyState_RootMotionScaler()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(81, 214, 141);
#endif

	RootMotionScale = 1.f;
	bNeedCheckCollision = false;
	ForwardDistance = 3.5f;
	TraceChannel = ETraceTypeQuery::TraceTypeQuery1;
	bDrawDebug = false;
}

FString UAnimNotifyState_RootMotionScaler::GetNotifyName_Implementation() const
{
	return TEXT("RM Scaler");
}

void UAnimNotifyState_RootMotionScaler::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	// 만약 어떤 이유로 초기화되지 않았을 가능성이 있으므로, 바인딩 해제
	// 실제 바인딩은 다음틱에서 적용됨
	UnbindRootMotionScalerFunction(MeshComp);
}

void UAnimNotifyState_RootMotionScaler::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	if (MeshComp == nullptr || MeshComp->GetOwner() == nullptr) return;

	if (ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner()))
	{
		if (DoCheck(MeshComp))
		{
			UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement();
			MovementComp->ProcessRootMotionPreConvertToWorld.BindUObject(this, &ThisClass::ConvertRootMotionDeltaLocation);
		}
		else
		{
			UnbindRootMotionScalerFunction(MeshComp);
		}
	}
}

void UAnimNotifyState_RootMotionScaler::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	UnbindRootMotionScalerFunction(MeshComp);
}

void UAnimNotifyState_RootMotionScaler::UnbindRootMotionScalerFunction(USkeletalMeshComponent* MeshComp)
{
	if (MeshComp == nullptr || MeshComp->GetOwner() == nullptr)
		return;

	if (ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner()))
	{
		UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement();
		MovementComp->ProcessRootMotionPreConvertToWorld.Unbind();
	}
}

bool UAnimNotifyState_RootMotionScaler::DoCheck(USkeletalMeshComponent* MeshComp) const
{
	if (MeshComp == nullptr || MeshComp->GetOwner() == nullptr)
		return false;

	if (bNeedCheckCollision == false)
		return true;
	
	// Check
	bool bCheck = false;
	if (ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner()))
	{
		FHitResult HitResult;
		TArray<AActor*> Ignores;
		Ignores.Add(Character);
		float CapsuleRadius		= Character->GetCapsuleComponent()->GetScaledCapsuleRadius()		- 2.5f;		// Padding (Hard Coded)
		float CapsuleHeight		= Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()	- 20.f;
		const FVector& Start	= Character->GetActorLocation();
		const FVector& End		= Start + (Character->GetActorForwardVector() * ForwardDistance);

		EDrawDebugTrace::Type DebugType = (bDrawDebug) ? EDrawDebugTrace::Type::ForOneFrame : EDrawDebugTrace::Type::None;
		bCheck = UKismetSystemLibrary::CapsuleTraceSingle(Character->GetWorld(), Start, End, CapsuleRadius, CapsuleHeight,
														TraceChannel, false, Ignores, DebugType, HitResult, true, FLinearColor::Red, FLinearColor::Green);
	}
	return bCheck;
}

FTransform UAnimNotifyState_RootMotionScaler::ConvertRootMotionDeltaLocation(const FTransform& InDeltaTransform, UCharacterMovementComponent* CharacterMovementComponent, float DeltaSeconds)
{
	FTransform Result = InDeltaTransform;
	Result.SetLocation(Result.GetLocation() * RootMotionScale);
	return Result;
}
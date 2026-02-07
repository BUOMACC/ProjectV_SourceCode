// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SideMovement.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_SideMovement::UBTTask_SideMovement()
{
	NodeName = TEXT("Side Movement(Left)");

	bCreateNodeInstance = true;
	INIT_TASK_NODE_NOTIFY_FLAGS();

	ExecuteTimeMinMax = FVector2D(2.f, 6.f);
	bIsLeftMovement = true;
	StopDistance = 250.f;

	CurrentTime = 0.f;
	TargetTime = 0.f;

	TraceCheckDistance = 120.f;
}

EBTNodeResult::Type UBTTask_SideMovement::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CurrentTime = 0.f;
	TargetTime = FMath::RandRange(ExecuteTimeMinMax.X, ExecuteTimeMinMax.Y);

	TurnSideMovementMode(OwnerComp.GetAIOwner()->GetPawn(), true);

	return EBTNodeResult::InProgress;
}

void UBTTask_SideMovement::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// Main Logic - Movement
	APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();
	FVector MoveDirection = OwnerPawn->GetActorRightVector();
	if (bIsLeftMovement)
		MoveDirection *= -1.f;

	OwnerPawn->AddMovementInput(MoveDirection, 1.f);

	// Finish Condition [1] - 이동중 장애물과 접촉된 경우
	FHitResult HitResult;
	const FVector& TraceStart = OwnerPawn->GetActorLocation();
	const FVector& TraceEnd = OwnerPawn->GetActorLocation() + (MoveDirection * TraceCheckDistance);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerPawn);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, Params))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	// Finish Condition [2] - Target이 설정된 경우, 타겟과 일정 거리 이내면 종료
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		if (UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()))
		{
			if (AActor* TargetActor = Cast<AActor>(TargetObject))
			{
				const FVector& OwnerLocation = OwnerPawn->GetActorLocation();
				const FVector& TargetLocation = TargetActor->GetActorLocation();

				float Distance = FVector::Distance(OwnerLocation, TargetLocation);
				if (Distance <= StopDistance)
				{
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					return;
				}
			}
		}
	}

	// Finish Condition [3] - 실행 시간만큼 모두 실행된 경우
	CurrentTime += DeltaSeconds;
	if (CurrentTime >= TargetTime)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
}

void UBTTask_SideMovement::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	TurnSideMovementMode(OwnerComp.GetOwner(), false);

	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

#if WITH_EDITOR
void UBTTask_SideMovement::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (bIsLeftMovement)
	{
		NodeName = TEXT("Side Movement(Left)");
	}
	else
	{
		NodeName = TEXT("Side Movement(Right)");
	}
}
#endif

void UBTTask_SideMovement::TurnSideMovementMode(AActor* Target, bool bEnableMode)
{
	//if (ACharacter* Character = Cast<ACharacter>(Target))
	//{
	//	if (UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement())
	//	{
	//		MovementComponent->bOrientRotationToMovement = !bEnableMode;
	//		MovementComponent->bUseControllerDesiredRotation = bEnableMode;
	//	}
	//}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_RotateTo.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Rotator.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_RotateTo::UBTTask_RotateTo()
{
	NodeName = TEXT("Rotate To");

	bCreateNodeInstance = true;
	INIT_TASK_NODE_NOTIFY_FLAGS();

	// Vector 또는 Actor만 허용
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_RotateTo, BlackboardKey), AActor::StaticClass());
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_RotateTo, BlackboardKey));
	BlackboardKey.AddRotatorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_RotateTo, BlackboardKey));

	StartRotation = FRotator::ZeroRotator;
	TargetRotation = FRotator::ZeroRotator;

	RotateTime = 0.5f;
	CurrentTime = 0.f;
}

EBTNodeResult::Type UBTTask_RotateTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (IsValid(OwnerComp.GetAIOwner()) == false)
	{
		return EBTNodeResult::Failed;
	}

	if (APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn())
	{
		StartRotation = ControlledPawn->GetActorRotation();
		if (UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent())
		{
			if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
			{
				UObject* KeyValue = Blackboard->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
				AActor* TargetActor = Cast<AActor>(KeyValue);
				if (TargetActor)
				{
					// Yaw 값만 사용
					const FRotator& RotationValue = UKismetMathLibrary::FindLookAtRotation(ControlledPawn->GetActorLocation(), TargetActor->GetActorLocation());
					TargetRotation = FRotator(0.f, RotationValue.Yaw, 0.f);
					return EBTNodeResult::InProgress;
				}
			}
			else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
			{
				const FVector& LocationValue = Blackboard->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());
				const FRotator& RotationValue = UKismetMathLibrary::FindLookAtRotation(ControlledPawn->GetActorLocation(), LocationValue);
				TargetRotation = FRotator(0.f, RotationValue.Yaw, 0.f);
				return EBTNodeResult::InProgress;
			}
			else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Rotator::StaticClass())
			{
				const FRotator& RotationValue = Blackboard->GetValue<UBlackboardKeyType_Rotator>(BlackboardKey.GetSelectedKeyID());
				TargetRotation = FRotator(0.f, RotationValue.Yaw, 0.f);
				return EBTNodeResult::InProgress;
			}
		}
	}
	return EBTNodeResult::Failed;
}

void UBTTask_RotateTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (IsValid(OwnerComp.GetAIOwner()) == false)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	if (CurrentTime >= RotateTime)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	CurrentTime += DeltaSeconds;
	float Ratio = FMath::Clamp(UKismetMathLibrary::SafeDivide(CurrentTime, RotateTime), 0.f, 1.f);
	if (APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn())
	{
		FRotator Rotation = UKismetMathLibrary::RLerp(StartRotation, TargetRotation, Ratio, true);
		ControlledPawn->SetActorRotation(Rotation);
	}
}

void UBTTask_RotateTo::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	StartRotation = FRotator::ZeroRotator;
	TargetRotation = FRotator::ZeroRotator;
	CurrentTime = 0.f;
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

#if WITH_EDITOR
void UBTTask_RotateTo::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

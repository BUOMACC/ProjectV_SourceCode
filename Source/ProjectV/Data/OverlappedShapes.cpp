// Fill out your copyright notice in the Description page of Project Settings.


#include "OverlappedShapes.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

/* ==================== Base ==================== */
UOverlappedShape_Base::UOverlappedShape_Base()
{
	RelativeLocation = FVector::ZeroVector;
	RelativeRotation = FRotator::ZeroRotator;
	TraceChannel = ETraceTypeQuery::TraceTypeQuery3;

	bDrawDebug = false;
}

bool UOverlappedShape_Base::CheckOverlap(AActor* ContextActor, TArray<FHitResult>& Results)
{
	return false;
}
/* ============================================== */


/* ==================== Box ==================== */
UOverlappedShape_Box::UOverlappedShape_Box()
{
	BoxSize = FVector(100.f, 100.f, 100.f);
}

bool UOverlappedShape_Box::CheckOverlap(AActor* ContextActor, TArray<FHitResult>& Results)
{
	if (ContextActor == nullptr || ContextActor->GetWorld()->WorldType == EWorldType::EditorPreview)
		return false;
	

	TArray<AActor*> IgnoreActor;
	IgnoreActor.Add(ContextActor);

	FTransform Transform = ContextActor->GetTransform();
	FVector ShapeLocation = UKismetMathLibrary::TransformLocation(Transform, RelativeLocation);
	FRotator ShapeRotation = UKismetMathLibrary::TransformRotation(Transform, RelativeRotation);

	bool bSuccess = UKismetSystemLibrary::BoxTraceMulti(ContextActor, ShapeLocation, ShapeLocation, BoxSize/2/* HalfSize */, ShapeRotation, TraceChannel, false,
														TArray<AActor*>(), EDrawDebugTrace::None, Results, true);

	// Debug
#if WITH_EDITOR
	if (bDrawDebug)
	{
		UKismetSystemLibrary::DrawDebugBox(ContextActor, ShapeLocation, BoxSize/2, FColor::Red, ShapeRotation, 1.f, 0.f);
	}
#endif

	return bSuccess;
}
/* ============================================= */
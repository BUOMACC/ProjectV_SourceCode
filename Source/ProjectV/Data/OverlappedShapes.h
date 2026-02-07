// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OverlappedShapes.generated.h"

class ACharacter;

/**
 *	Overlap을 감지하는 도형의 기본이 되는 클래스
 */
UCLASS(Blueprintable, BlueprintType)
class PROJECTV_API UOverlappedShape_Base : public UObject
{
	GENERATED_BODY()
	
public:
	UOverlappedShape_Base();
	virtual bool CheckOverlap(AActor* ContextActor, TArray<FHitResult>& Results);

public:
	/* 도형의 상대 위치 */
	UPROPERTY(EditAnywhere, Category = "Shape")
	FVector RelativeLocation;

	/* 도형의 상대 회전값 */
	UPROPERTY(EditAnywhere, Category = "Shape")
	FRotator RelativeRotation;

	/* Trace에 사용할 채널 */
	UPROPERTY(EditAnywhere, Category = "Shape")
	TEnumAsByte<ETraceTypeQuery> TraceChannel;

	/* Debugging */
	UPROPERTY(EditAnywhere, Category = "Shape")
	bool bDrawDebug;
};


/*
*	Shape : Box
*/
UCLASS(EditInlineNew)
class PROJECTV_API UOverlappedShape_Box : public UOverlappedShape_Base
{
	GENERATED_BODY()

public:
	UOverlappedShape_Box();
	virtual bool CheckOverlap(AActor* ContextActor, TArray<FHitResult>& Results) override;

public:
	UPROPERTY(EditAnywhere, Category = "Shape")
	FVector BoxSize;
};


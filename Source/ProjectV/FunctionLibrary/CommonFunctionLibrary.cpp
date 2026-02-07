// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"

FVector UCommonFunctionLibrary::RandomVectorRange(const FVector& Vec1, const FVector& Vec2)
{
	FVector Result = FVector::ZeroVector;
	Result.X = FMath::RandRange(Vec1.X, Vec2.X);
	Result.Y = FMath::RandRange(Vec1.Y, Vec2.Y);
	Result.Z = FMath::RandRange(Vec1.Z, Vec2.Z);

	return Result;
}

FRotator UCommonFunctionLibrary::RandomRotatorRange(const FRotator& Rot1, const FRotator& Rot2)
{
	FRotator Result = FRotator::ZeroRotator;
	Result.Roll = FMath::RandRange(Rot1.Roll, Rot2.Roll);
	Result.Pitch = FMath::RandRange(Rot1.Pitch, Rot2.Pitch);
	Result.Yaw = FMath::RandRange(Rot1.Yaw, Rot2.Yaw);

	return Result;
}

float UCommonFunctionLibrary::GetTwoActorAngle(const AActor* Pivot, const AActor* Target, bool bIgnoreSign)
{
	if (Pivot == nullptr || Target == nullptr)
		return 0.f;

	float Angle = 0.f;

	const FVector& PivotForwardDir = Pivot->GetActorForwardVector();
	const FVector& PivotToTargetDir = (Target->GetActorLocation() - Pivot->GetTargetLocation()).GetSafeNormal();

	float DotScalar = FMath::Acos(FVector::DotProduct(PivotForwardDir, PivotToTargetDir));
	Angle = FMath::RadiansToDegrees(DotScalar);

	if (!bIgnoreSign)
	{
		FVector Cross = FVector::CrossProduct(PivotForwardDir, PivotToTargetDir);
		if (Cross.Z < 0.f)
		{
			Angle = Angle * -1;
		}
	}

	return Angle;
}
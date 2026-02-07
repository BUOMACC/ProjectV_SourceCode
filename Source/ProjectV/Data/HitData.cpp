// Fill out your copyright notice in the Description page of Project Settings.


#include "HitData.h"
#include "CombatGameplayTags.h"
#include "Kismet/KismetMathLibrary.h"

UHitData::UHitData()
{
	AttackDirection = FCombatGameplayTags::Get().AttackDirection_Center;
	HitMainType = FCombatGameplayTags::Get().Hit_MainType_Small;
	bCanParry = true;
}

FVector UHitData::GetPushDirection(AActor* Attacker, AActor* Victim) const
{
	return CalculatePushDirection(Attacker, Victim, PushType, PushAmount);
}

FVector UHitData::GetGuardPushDirection(AActor* Attacker, AActor* Victim) const
{
	return CalculatePushDirection(Attacker, Victim, GuardPushType, GuardPushAmount);
}

FVector UHitData::CalculatePushDirection(AActor* Attacker, AActor* Victim, EPushType InPushType, FVector InPushAmount) const
{
	// 공격자 또는 피격자가 비어있는 경우 예외처리
	if (Attacker == nullptr || Victim == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UHitData::CalculatePushDirection -> Attacker or Victim is nullptr."));
		return FVector::ZeroVector;
	}

	// calculate push direction
	FVector Direction = FVector::ZeroVector;
	switch (InPushType)
	{
	case EPushType::None:
		break;

	case EPushType::Attacker:
		Direction = UKismetMathLibrary::TransformDirection(Attacker->GetTransform(), InPushAmount);
		break;

	case EPushType::Victim:
		Direction = UKismetMathLibrary::TransformDirection(Victim->GetTransform(), InPushAmount);
		break;

	case EPushType::World:
		Direction = InPushAmount;
		break;

	case EPushType::AttackerToVictim:
		FRotator Rot = (Victim->GetActorLocation() - Attacker->GetActorLocation()).ToOrientationRotator();
		FTransform Transform = FTransform(Rot, Attacker->GetActorLocation());
		Direction = UKismetMathLibrary::TransformDirection(Transform, InPushAmount);
		break;
	}

	return Direction;
}
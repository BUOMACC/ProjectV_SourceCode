// Fill out your copyright notice in the Description page of Project Settings.


#include "BranchCondition_HasAcceleration.h"
#include "GameFramework/CharacterMovementComponent.h"

bool UBranchCondition_HasAcceleration::CheckCondition(ACombatCharacter* Instigator)
{
	bool Result = false;

	UCharacterMovementComponent* MovementComp = Instigator->GetCharacterMovement();
	if (MovementComp == false)
		return false;

	if (MovementComp->GetCurrentAcceleration().Size2D() > 0.f)
	{
		Result = true;
	}

	return Result;
}
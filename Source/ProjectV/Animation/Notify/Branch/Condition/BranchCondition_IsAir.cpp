// Fill out your copyright notice in the Description page of Project Settings.


#include "BranchCondition_IsAir.h"
#include "GameFramework/CharacterMovementComponent.h"

bool UBranchCondition_IsAir::CheckCondition(ACombatCharacter* Instigator)
{
	bool Result = false;

	UCharacterMovementComponent* MovementComp = Instigator->GetCharacterMovement();
	if (MovementComp == false)
		return false;

	if (MovementComp->IsFalling())
	{
		Result = true;
	}

	return Result;
}

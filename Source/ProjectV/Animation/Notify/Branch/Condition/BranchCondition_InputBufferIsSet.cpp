// Fill out your copyright notice in the Description page of Project Settings.


#include "BranchCondition_InputBufferIsSet.h"
#include "Character/PlayerComponent.h"
#include "Character/PlayerCharacter.h"

bool UBranchCondition_InputBufferIsSet::CheckCondition(ACombatCharacter* Instigator)
{
	bool Result = false;

	// --------------------------------------------------------------
	//	예외 - 조건 체크가 불가능한 경우, false
	APlayerCharacter* Player = Cast<APlayerCharacter>(Instigator);
	if (Player == nullptr)
		return false;

	UPlayerComponent* PlayerComp = Player->GetPlayerComponent();
	if (PlayerComp == false)
		return false;
	// --------------------------------------------------------------

	const FGameplayTag& InputBuffer = PlayerComp->GetInputBuffer();
	if (InputBuffer.MatchesTagExact(InputTag))
	{
		Result = true;
	}

	return Result;
}
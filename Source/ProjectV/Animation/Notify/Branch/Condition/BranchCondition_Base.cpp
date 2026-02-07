// Fill out your copyright notice in the Description page of Project Settings.


#include "BranchCondition_Base.h"

UBranchCondition_Base::UBranchCondition_Base()
{
	bReverseResult = false;
}

bool UBranchCondition_Base::CheckCondition(ACombatCharacter* Instigator)
{
	return true;
}
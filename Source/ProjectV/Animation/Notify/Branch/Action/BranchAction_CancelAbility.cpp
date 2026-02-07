// Fill out your copyright notice in the Description page of Project Settings.


#include "BranchAction_CancelAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Ability/ProjectVGameplayAbility.h"

UBranchAction_CancelAbility::UBranchAction_CancelAbility()
{

}

void UBranchAction_CancelAbility::Activate(ACombatCharacter* Instigator)
{
	if (Instigator)
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Instigator))
		{
			ASC->CancelAbilities(&CancelAbilityTags);
		}
	}
}
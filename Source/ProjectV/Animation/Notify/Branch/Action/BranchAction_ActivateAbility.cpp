// Fill out your copyright notice in the Description page of Project Settings.


#include "BranchAction_ActivateAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Ability/ProjectVGameplayAbility.h"

UBranchAction_ActivateAbility::UBranchAction_ActivateAbility()
{

}

void UBranchAction_ActivateAbility::Activate(ACombatCharacter* Instigator)
{
	if (Instigator)
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Instigator))
		{
			ASC->TryActivateAbilityByClass(AbilityClass);
		}
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "BranchAction_SendGameplayEvent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Ability/ProjectVGameplayAbility.h"

UBranchAction_SendGameplayEvent::UBranchAction_SendGameplayEvent()
{

}

void UBranchAction_SendGameplayEvent::Activate(ACombatCharacter* Instigator)
{
	if (Instigator)
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Instigator))
		{
			FGameplayEventData PayloadData;
			PayloadData.Instigator = Instigator;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Instigator, GameEventTag, PayloadData);
		}
	}
}
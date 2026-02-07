// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDefinition.h"
#include "CombatGameplayTags.h"

UItemDefinition::UItemDefinition()
{
	ItemFragments.Empty();
}

UItemDefinition* UItemDefinition::GetItemDefCDO(TSubclassOf<UItemDefinition> ItemDefinitionClass)
{
	if (ItemDefinitionClass)
	{
		return Cast<UItemDefinition>(ItemDefinitionClass->GetDefaultObject());
	}
	return nullptr;
}

const UItemFragmentBase* UItemDefinition::FindItemFragmentByClass(TSubclassOf<UItemFragmentBase> ItemFragmentClass) const
{
	if (ItemFragmentClass != nullptr)
	{
		for (UItemFragmentBase* ItemFragment : ItemFragments)
		{
			if (ItemFragment && ItemFragment->IsA(ItemFragmentClass))
			{
				return ItemFragment;
			}
		}
	}
	return nullptr;
}

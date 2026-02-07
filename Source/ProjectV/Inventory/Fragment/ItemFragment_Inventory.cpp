// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemFragment_Inventory.h"

UItemFragment_Inventory::UItemFragment_Inventory()
{
	DisplayIcon = nullptr;
	DisplayName = FText::GetEmpty();
	DisplayDescription = FText::GetEmpty();
	// ItemType = FGameplayTag::EmptyTag;
	ItemTier = EItemTier::Normal;
}
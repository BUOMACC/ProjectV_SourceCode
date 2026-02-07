// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInstance.h"
#include "Net/UnrealNetwork.h"
#include "Inventory/ItemDefinition.h"

UItemInstance::UItemInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Count = 0;
}

void UItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemDef);
	DOREPLIFETIME(ThisClass, Count);
	DOREPLIFETIME(ThisClass, LastCount);
}

void UItemInstance::SetItemDef(TSubclassOf<UItemDefinition> InDef)
{
	ItemDef = InDef;
}

TSubclassOf<UItemDefinition> UItemInstance::GetItemDef() const
{
	return ItemDef;
}

void UItemInstance::AddCount(int32 CountToAdd)
{
	LastCount = Count;
	Count += CountToAdd;
}

void UItemInstance::SetCount(int32 NewCount)
{
	LastCount = Count;
	NewCount = FMath::Clamp(NewCount, 0, NewCount);
	Count = NewCount;
}

int32 UItemInstance::GetCount() const
{
	return Count;
}

int32 UItemInstance::GetLastCount() const
{
	return LastCount;
}

const UItemFragmentBase* UItemInstance::FindItemFragmentByClass(TSubclassOf<UItemFragmentBase> ItemFragmentClass) const
{
	if ((ItemDef != nullptr) && (ItemFragmentClass != nullptr))
	{
		return GetDefault<UItemDefinition>(ItemDef)->FindItemFragmentByClass(ItemFragmentClass);
	}

	return nullptr;
}
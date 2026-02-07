// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "ItemDefinition.h"
#include "ItemInstance.h"
#include "ProjectVLogs.h"

#include "Fragment/ItemFragment_LimitStack.h"

#define INVENTORY_LOG(Verbosity, Format, ...) UE_LOG(LogInventory, Verbosity, TEXT("[InventoryComponent] - %s"), *FString::Printf(Format, ##__VA_ARGS__))

FInventoryItemEntry::FInventoryItemEntry()
	: ItemInstance(false)
{
	// ...
}


FInventoryItemList::FInventoryItemList()
{
	
}

FInventoryItemList::FInventoryItemList(UInventoryComponent* InOwnerComponent)
	: OwnerComponent(InOwnerComponent)
{

}

void FInventoryItemList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		const FInventoryItemEntry& Entry = Entries[Index];
		if (OwnerComponent)
		{
			OwnerComponent->BroadcastItemChangeMessage(Entry, Entry.ItemInstance->GetCount(), 0);
		}
	}
}

void FInventoryItemList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		const FInventoryItemEntry& Entry = Entries[Index];
		if (OwnerComponent)
		{
			OwnerComponent->BroadcastItemChangeMessage(Entry, 0, Entry.ItemInstance->GetCount());
		}
	}
}

void FInventoryItemList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (int32 Index : ChangedIndices)
	{
		const FInventoryItemEntry& Entry = Entries[Index];
		if (OwnerComponent)
		{
			OwnerComponent->BroadcastItemChangeMessage(Entry, Entry.ItemInstance->GetLastCount(), Entry.ItemInstance->GetCount());
		}
	}
}

UItemInstance* FInventoryItemList::AddEntry(TSubclassOf<UItemDefinition> ItemDef, int32 ItemCount)
{
	UItemInstance* Result = nullptr;

	check(ItemDef != nullptr);
	check(OwnerComponent);

	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());	// only server

	FInventoryItemEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.ItemInstance = NewObject<UItemInstance>(OwningActor);
	NewEntry.ItemInstance->SetItemDef(ItemDef);
	for (UItemFragmentBase* ItemFragment : GetDefault<UItemDefinition>(ItemDef)->ItemFragments)
	{
		if (ItemFragment != nullptr)
		{
			ItemFragment->OnInstanceCreated(NewEntry.ItemInstance);
		}
	}
	NewEntry.ItemInstance->SetCount(ItemCount);
	Result = NewEntry.ItemInstance;

	if (OwnerComponent)
	{
		OwnerComponent->BroadcastItemChangeMessage(NewEntry, NewEntry.ItemInstance->GetLastCount(), NewEntry.ItemInstance->GetCount());
	}
	MarkItemDirty(NewEntry);

	return Result;
}

void FInventoryItemList::RemoveEntry(UItemInstance* InItemInstance)
{
	// Iterator로 처리하면 항목이 삭제된 이후 순회시에 용이합니다.
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FInventoryItemEntry& Entry = *EntryIt;
		if (Entry.ItemInstance == InItemInstance)
		{
			if (OwnerComponent)
			{
				OwnerComponent->BroadcastItemChangeMessage(Entry, Entry.ItemInstance->GetLastCount(), 0);
			}
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

TArray<UItemInstance*> FInventoryItemList::GetAllItems() const
{
	TArray<UItemInstance*> Results;
	Results.Reserve(Entries.Num());
	for (const FInventoryItemEntry& Entry : Entries)
	{
		if (Entry.ItemInstance != nullptr)
		{
			Results.Add(Entry.ItemInstance);
		}
	}
	return Results;
}


UInventoryComponent::UInventoryComponent()
	: InventoryList(this)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UInventoryComponent, InventoryList, COND_OwnerOnly);
}

UItemInstance* UInventoryComponent::AddItemDefinition(TSubclassOf<UItemDefinition> ItemDef, int32 AddCount)
{
	// Only Server
	if (OwnerHasAuthority() == false)
		return nullptr;

	// Exception
	if (!IsValid(ItemDef) || AddCount <= 0)
		return nullptr;

	const UItemDefinition* ItemDefCDO = GetDefault<UItemDefinition>(ItemDef);
	const UItemFragment_LimitStack* ItemFragment = ItemDefCDO->FindItemFragmentByClass<UItemFragment_LimitStack>();
	int32 MaxStackCount = (ItemFragment) ? ItemFragment->MaxStackCount : 1;

	// 1) Stacking 가능한 인스턴스가 있는경우, 카운트 증가처리
	UItemInstance* Result = nullptr;
	for (FInventoryItemEntry& Entry : InventoryList.Entries)
	{
		UItemInstance* InstancesToStack = Entry.ItemInstance;
		if (!IsValid(InstancesToStack) || InstancesToStack->GetItemDef() != ItemDef)
			continue;

		// Entry에 추가해야할 개수와, 추가하고 남은 개수 계산
		int32 CountToAdd = 0;
		bool bIsStackOver = ((InstancesToStack->GetCount() + AddCount) > MaxStackCount);
		if (bIsStackOver)
		{
			CountToAdd = MaxStackCount - InstancesToStack->GetCount();
		}
		else
		{
			CountToAdd = AddCount;
		}

		// 항목에 개수 추가
		if (CountToAdd > 0)
		{
			InstancesToStack->AddCount(CountToAdd);

			BroadcastItemChangeMessage(Entry, InstancesToStack->GetLastCount(), InstancesToStack->GetCount());
			InventoryList.MarkItemDirty(Entry);

			Result = InstancesToStack;
			AddCount = (AddCount - CountToAdd);
			if (AddCount <= 0)
			{
				AddCount = 0;
				break;
			}
		}
	}

	// 2) 남은 개수들은 새로운 인스턴스 생성
	while (AddCount > 0)
	{
		int32 AdjustedAddingCount = FMath::Clamp(AddCount, 0, MaxStackCount);
		Result = InventoryList.AddEntry(ItemDef, AdjustedAddingCount);
		if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && Result)
		{
			AddReplicatedSubObject(Result);
		}

		AddCount -= AdjustedAddingCount;
	}

	return Result;
}

void UInventoryComponent::AddItemCount(UItemInstance* ItemInstance, int32 AddCount)
{
	// Only Server
	if (OwnerHasAuthority() == false)
		return;

	// Exception
	if (!IsValid(ItemInstance) || AddCount <= 0)
		return;

	const UItemDefinition* ItemDefCDO = GetDefault<UItemDefinition>(ItemInstance->GetItemDef());
	const UItemFragment_LimitStack* ItemFragment = ItemDefCDO->FindItemFragmentByClass<UItemFragment_LimitStack>();
	int32 MaxStackCount = (ItemFragment) ? ItemFragment->MaxStackCount : 1;

	for (FInventoryItemEntry& Entry : InventoryList.Entries)
	{
		UItemInstance* InstancesToStack = Entry.ItemInstance;
		if (!IsValid(InstancesToStack) || InstancesToStack != ItemInstance)
			continue;

		int32 CountToAdd = 0;
		bool bIsStackOver = ((InstancesToStack->GetCount() + AddCount) > MaxStackCount);
		if (bIsStackOver)
		{
			CountToAdd = MaxStackCount - InstancesToStack->GetCount();
		}
		else
		{
			CountToAdd = AddCount;
		}

		// 항목에 개수 추가
		InstancesToStack->AddCount(CountToAdd);

		BroadcastItemChangeMessage(Entry, InstancesToStack->GetLastCount(), InstancesToStack->GetCount());
		InventoryList.MarkItemDirty(Entry);
	}
}

void UInventoryComponent::RemoveItemDefinition(TSubclassOf<UItemDefinition> ItemDef, int32 RemoveCount)
{
	// Only Server
	if (OwnerHasAuthority() == false)
		return;

	// Exception
	if (!IsValid(ItemDef) || RemoveCount <= 0)
		return;

	// 1) Stacking 가능한 인스턴스가 있는경우, 카운트 감소처리
	for (int32 i = InventoryList.Entries.Num() - 1; i >= 0; i--)
	{
		FInventoryItemEntry& Entry = InventoryList.Entries[i];
		UItemInstance* InstancesToRemove = Entry.ItemInstance;
		if (!IsValid(InstancesToRemove) || InstancesToRemove->GetItemDef() != ItemDef)
			continue;

		// Entry에 삭제해야할 개수와, 삭제하고 남은 개수 계산
		int32 CountToRemove = 0;
		bool bIsStackUnder = ((InstancesToRemove->GetCount() - RemoveCount) <= 0);
		if (bIsStackUnder)
		{
			CountToRemove = InstancesToRemove->GetCount();
		}
		else
		{
			CountToRemove = RemoveCount;
		}

		// 항목에 개수 감소
		if (CountToRemove > 0)
		{
			InstancesToRemove->AddCount(-CountToRemove);

			BroadcastItemChangeMessage(Entry, InstancesToRemove->GetLastCount(), InstancesToRemove->GetCount());
			InventoryList.MarkItemDirty(Entry);

			// 더이상 유효한 개수가 아닌경우 -> 삭제
			if (InstancesToRemove->GetCount() <= 0)
			{
				InventoryList.RemoveEntry(InstancesToRemove);
				if (InstancesToRemove && IsUsingRegisteredSubObjectList())
				{
					RemoveReplicatedSubObject(InstancesToRemove);
				}
			}

			RemoveCount = (RemoveCount - CountToRemove);
			if (RemoveCount <= 0)
			{
				RemoveCount = 0;
				break;
			}
		}
	}
}

void UInventoryComponent::RemoveItemCount(UItemInstance* ItemInstance, int32 RemoveCount)
{
	// Only Server
	if (OwnerHasAuthority() == false)
		return;

	// Exception
	if (!IsValid(ItemInstance) || RemoveCount <= 0)
		return;

	for (int32 i = InventoryList.Entries.Num() - 1; i >= 0; i--)
	{
		FInventoryItemEntry& Entry = InventoryList.Entries[i];
		UItemInstance* InstancesToRemove = Entry.ItemInstance;
		if (!IsValid(InstancesToRemove) || InstancesToRemove != ItemInstance)
			continue;

		int32 CountToRemove = 0;
		bool bIsStackUnder = ((InstancesToRemove->GetCount() - RemoveCount) <= 0);
		if (bIsStackUnder)
		{
			CountToRemove = InstancesToRemove->GetCount();
		}
		else
		{
			CountToRemove = RemoveCount;
		}

		// 항목에 개수 추가
		InstancesToRemove->AddCount(-CountToRemove);

		BroadcastItemChangeMessage(Entry, InstancesToRemove->GetLastCount(), InstancesToRemove->GetCount());
		InventoryList.MarkItemDirty(Entry);

		// 더이상 유효한 개수가 아닌경우 -> 삭제
		if (InstancesToRemove->GetCount() <= 0)
		{
			InventoryList.RemoveEntry(InstancesToRemove);
			if (InstancesToRemove && IsUsingRegisteredSubObjectList())
			{
				RemoveReplicatedSubObject(InstancesToRemove);
			}
		}
	}
}

TArray<UItemInstance*> UInventoryComponent::GetAllItems() const
{
	return InventoryList.GetAllItems();
}

int32 UInventoryComponent::GetItemCount(TSubclassOf<UItemDefinition> ItemDef) const
{
	int32 Result = 0;
	const TArray<UItemInstance*> Items = GetAllItems();
	for (const UItemInstance* Item : Items)
	{
		if (Item->GetItemDef() == ItemDef)
		{
			Result += Item->GetCount();
		}
	}

	return Result;
}

void UInventoryComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	if (IsUsingRegisteredSubObjectList())
	{
		for (const FInventoryItemEntry& Entry : InventoryList.Entries)
		{
			UItemInstance* Instance = Entry.ItemInstance;

			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}

bool UInventoryComponent::ReplicateSubobjects(UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FInventoryItemEntry& Entry : InventoryList.Entries)
	{
		UItemInstance* Instance = Entry.ItemInstance;

		if (Instance && IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

bool UInventoryComponent::OwnerHasAuthority() const
{
	if (GetOwner() == false)
		return false;

	return GetOwner()->HasAuthority();
}

void UInventoryComponent::BroadcastItemChangeMessage(const FInventoryItemEntry& Entry, int32 OldCount, int32 NewCount)
{
	if (OnInventoryItemChanged.IsBound())
	{
		OnInventoryItemChanged.Broadcast(Entry.ItemInstance, OldCount, NewCount);
	}
}

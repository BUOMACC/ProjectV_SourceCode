// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickSlotComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/CombatAbilitySystemComponent.h"
#include "CombatGameplayTags.h"
#include "Equipment/EquipmentComponent.h"
#include "FunctionLibrary/CommonFunctionLibrary.h"
#include "Inventory/ItemInstance.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/Fragment/ItemFragment_Equipment.h"
#include "Inventory/Fragment/ItemFragment_QuickSlot.h"
#include "Inventory/Fragment/ItemFragment_Useable.h"
#include "Inventory/Fragment/ItemFragment_Skill.h"
#include "Net/UnrealNetwork.h"
#include "ProjectVLogs.h"

/////////////////////////////////////////////////////////////////////////
//	Registered QuickSlot Info
FRegisteredQuickSlotInfo::FRegisteredQuickSlotInfo()
	: SlotType(EQuickSlotType::None),
	  SlotSize(1)
{
}

/////////////////////////////////////////////////////////////////////////
//	QuickSlot
FQuickSlot::FQuickSlot()
	: SlotType(EQuickSlotType::None),
	  ActiveSlotIndex(0)
{
}

int32 FQuickSlot::GetSlotSize()
{
	return Slots.Num();
}

int32 FQuickSlot::GetEmptySlotIndex()
{
	for (int32 i = 0; i < GetSlotSize(); i++)
	{
		if (Slots[i] == nullptr)
		{
			return i;
		}
	}
	return INDEX_NONE;
}

bool FQuickSlot::IsValidIndex(int32 Index)
{
	return (Index >= 0 && Index < GetSlotSize());
}

bool FQuickSlot::HasItem(UItemInstance* ItemInstance)
{
	return Slots.Contains(ItemInstance);
}

int32 FQuickSlot::FindSlotIndex(UItemInstance* ItemInstance)
{
	int32 Result = INDEX_NONE;
	Slots.Find(ItemInstance, Result);

	return Result;
}

/////////////////////////////////////////////////////////////////////////
//	QuickSlot Component
UQuickSlotComponent::UQuickSlotComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
}

void UQuickSlotComponent::BeginPlay()
{
	Super::BeginPlay();

	GetInventoryComponent()->OnInventoryItemChanged.AddDynamic(this, &UQuickSlotComponent::OnInventoryItemChanged);
}

void UQuickSlotComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	CreateQuickSlots();
}

void UQuickSlotComponent::UninitializeComponent()
{
	Super::UninitializeComponent();
}

void UQuickSlotComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(ThisClass, QuickSlotList, COND_OwnerOnly);
}

void UQuickSlotComponent::CreateQuickSlots()
{
	if (!GetOwner()->HasAuthority())
		return;

	for (const FRegisteredQuickSlotInfo& Info : RegisteredInfo)
	{
		if (Info.SlotType == EQuickSlotType::None)
		{
			UE_LOG(LogQuickSlot, Warning, TEXT("[%s] 타입에 대한 퀵슬롯을 생성할 수 없습니다. 잘못된 타입입니다."), *UCommonFunctionLibrary::GetEnumNameString<EQuickSlotType>(Info.SlotType));
			return;
		}

		if (GetQuickSlot(Info.SlotType))
		{
			UE_LOG(LogQuickSlot, Warning, TEXT("[%s] 타입에 대한 퀵슬롯을 생성할 수 없습니다. 이미 존재하는 퀵슬롯입니다."), *UCommonFunctionLibrary::GetEnumNameString<EQuickSlotType>(Info.SlotType));
			return;
		}

		// Create Quick Slot
		FQuickSlot& NewElement = QuickSlotList.AddDefaulted_GetRef();
		NewElement.SlotType = Info.SlotType;
		NewElement.Slots.SetNum(Info.SlotSize);
		NewElement.ActiveSlotIndex = 0;
	}

	OnQuickSlotInitialized.Broadcast();
}

void UQuickSlotComponent::ApplyActiveQuickSlotState(EQuickSlotType InSlotType, UItemInstance* OldItem, UItemInstance* NewItem)
{
	FQuickSlot* QuickSlot = GetQuickSlot(InSlotType);
	if (QuickSlot == nullptr)
	{
		return;
	}

	// 동일한 아이템으로 변경된 경우, 처리 없음
	if (OldItem && NewItem && OldItem == NewItem)
	{
		return;
	}

	if (OldItem)
	{
		switch (InSlotType)
		{
		case EQuickSlotType::Weapon:
			UnEquipWeapon();
			break;

		case EQuickSlotType::Consumption:
			GetAbilitySystemComponent()->TakeGrantAbilityData(FCombatGameplayTags::Get().GrantAbility_Consume);
			break;

		case EQuickSlotType::Skill:
			GetAbilitySystemComponent()->TakeGrantAbilityData(FCombatGameplayTags::Get().GrantAbility_Skill);
			break;
		}
	}

	if (NewItem)
	{
		switch (InSlotType)
		{
		case EQuickSlotType::Weapon:
			EquipWeapon();
			break;

		case EQuickSlotType::Consumption:
			if (const UItemFragment_Useable* UseableFragment = NewItem->FindItemFragmentByClass<UItemFragment_Useable>())
			{
				GetAbilitySystemComponent()->ApplyGrantAbilityData(UseableFragment->AbilityToGrant, NewItem);
			}
			break;

		case EQuickSlotType::Skill:
			if (const UItemFragment_Skill* SkillFragment = NewItem->FindItemFragmentByClass<UItemFragment_Skill>())
			{
				GetAbilitySystemComponent()->ApplyGrantAbilityData(SkillFragment->AbilityToGrant, NewItem);
			}
			break;
		}
	}
}

UInventoryComponent* UQuickSlotComponent::GetInventoryComponent()
{
	if (CachedInventoryComponent)
	{
		return CachedInventoryComponent;
	}

	// 만약 없다면, 찾아서 캐싱해둠
	if (AActor* OwnerActor = GetOwner())
	{
		if (UInventoryComponent* InventoryComponent = OwnerActor->FindComponentByClass<UInventoryComponent>())
		{
			CachedInventoryComponent = InventoryComponent;
			return CachedInventoryComponent;
		}
	}
	return nullptr;
}

UEquipmentComponent* UQuickSlotComponent::GetEquipmentComponent()
{
	if (CachedEquipmentComponent)
	{
		return CachedEquipmentComponent;
	}

	// 만약 없다면, 찾아서 캐싱해둠
	if (AActor* OwnerActor = GetOwner())
	{
		if (UEquipmentComponent* EquipmentComponent = OwnerActor->FindComponentByClass<UEquipmentComponent>())
		{
			CachedEquipmentComponent = EquipmentComponent;
			return CachedEquipmentComponent;
		}
	}
	return nullptr;
}

void UQuickSlotComponent::OnInventoryItemChanged(UItemInstance* ItemInstance, int32 OldCount, int32 ItemCount)
{
	if (GetOwner() == nullptr || GetOwner()->HasAuthority() == false)
	{
		return;
	}

	/* 인벤토리에서 아이템이 지워진 경우, 퀵슬롯에서 등록을 해제합니다. */
	if (IsValid(ItemInstance) && ItemCount <= 0)
	{
		if (const UItemFragment_QuickSlot* QuickSlotFragment = ItemInstance->FindItemFragmentByClass<UItemFragment_QuickSlot>())
		{
			if (FQuickSlot* QuickSlot = GetQuickSlot(QuickSlotFragment->SlotType))
			{
				int32 IndexToRemove = QuickSlot->FindSlotIndex(ItemInstance);
				if (IndexToRemove != INDEX_NONE)
				{
					RemoveFromQuickSlot(QuickSlotFragment->SlotType, IndexToRemove);
				}
			}
		}
	}
}

void UQuickSlotComponent::AddToQuickSlot(UItemInstance* InItemInstance, EQuickSlotType InSlotType, int32 InSlotIndex)
{
	if (CanAddToQuickSlot(InItemInstance, InSlotType, InSlotIndex))
	{
		AddToQuickSlot_Server(InItemInstance, InSlotType, InSlotIndex);
	}
}

void UQuickSlotComponent::RemoveFromQuickSlot(EQuickSlotType InSlotType, int32 InSlotIndex)
{
	if (CanRemoveFromQuickSlot(InSlotType, InSlotIndex))
	{
		RemoveFromQuickSlot_Server(InSlotType, InSlotIndex);
	}
}

bool UQuickSlotComponent::ClearQuickSlot(EQuickSlotType InSlotType)
{
	FQuickSlot* QuickSlot = GetQuickSlot(InSlotType);
	if (QuickSlot == nullptr)
	{
		return false;
	}

	ClearQuickSlot_Server(InSlotType);
	return true;
}

void UQuickSlotComponent::AddToQuickSlot_Server_Implementation(UItemInstance* InItemInstance, EQuickSlotType InSlotType, int32 InSlotIndex)
{
	if (!CanAddToQuickSlot(InItemInstance, InSlotType, InSlotIndex))
		return;

	if (const UItemFragment_QuickSlot* QuickSlotFragment = InItemInstance->FindItemFragmentByClass<UItemFragment_QuickSlot>())
	{
		if (FQuickSlot* QuickSlot = GetQuickSlot(QuickSlotFragment->SlotType))
		{
			TArray<FQuickSlot> PrevQuickSlotList = QuickSlotList;

			// 이미 등록된 아이템인 경우, 등록된 인덱스에 해당하는 퀵슬롯은 비웁니다.
			int32 PrevItemSlotIndex = QuickSlot->FindSlotIndex(InItemInstance);
			if (PrevItemSlotIndex != INDEX_NONE)
			{
				RemoveFromQuickSlot(QuickSlotFragment->SlotType, PrevItemSlotIndex);
			}

			// 아이템을 퀵슬롯에 등록합니다.
			UItemInstance* OldItem = QuickSlot->Slots[InSlotIndex];
			QuickSlot->Slots[InSlotIndex] = InItemInstance;

			// 활성화 슬롯을 조작한 경우에만 어빌리티 부여 등의 행위가 일어남
			if (InSlotIndex == QuickSlot->ActiveSlotIndex)
			{
				ApplyActiveQuickSlotState(InSlotType, OldItem, QuickSlot->Slots[InSlotIndex]/* NewItem */);
			}

			OnRep_QuickSlotList(PrevQuickSlotList);
		}
	}
}

void UQuickSlotComponent::RemoveFromQuickSlot_Server_Implementation(EQuickSlotType InSlotType, int32 InSlotIndex)
{
	if (!CanRemoveFromQuickSlot(InSlotType, InSlotIndex))
		return;

	if (FQuickSlot* QuickSlot = GetQuickSlot(InSlotType))
	{
		TArray<FQuickSlot> PrevQuickSlotList = QuickSlotList;
		UItemInstance* OldItem = QuickSlot->Slots[InSlotIndex];
		QuickSlot->Slots[InSlotIndex] = nullptr;

		if (InSlotIndex == QuickSlot->ActiveSlotIndex)
		{
			ApplyActiveQuickSlotState(InSlotType, OldItem, QuickSlot->Slots[InSlotIndex]/* NewItem */);
		}

		OnRep_QuickSlotList(PrevQuickSlotList);
	}
}

void UQuickSlotComponent::ClearQuickSlot_Server_Implementation(EQuickSlotType InSlotType)
{
	if (FQuickSlot* QuickSlot = GetQuickSlot(InSlotType))
	{
		TArray<FQuickSlot> PrevQuickSlotList = QuickSlotList;
		for (int32 i = 0; i < QuickSlot->GetSlotSize(); i++)
		{
			QuickSlot->Slots[i] = nullptr;
		}

		ApplyActiveQuickSlotState(InSlotType, nullptr, nullptr);

		OnRep_QuickSlotList(PrevQuickSlotList);
	}
}

bool UQuickSlotComponent::CanAddToQuickSlot(UItemInstance* InItemInstance, EQuickSlotType InSlotType, int32 InSlotIndex)
{
	if (!IsValid(InItemInstance))
	{
		return false;
	}

	const UItemFragment_QuickSlot* QuickSlotFragment = InItemInstance->FindItemFragmentByClass<UItemFragment_QuickSlot>();
	if (!QuickSlotFragment)
	{
		return false;
	}

	// 해당 아이템이 퀵슬롯에 호환되는지 체크
	if (QuickSlotFragment->SlotType != InSlotType)
	{
		return false;
	}

	FQuickSlot* QuickSlot = GetQuickSlot(InSlotType);
	if (QuickSlot == nullptr)
	{
		return false;
	}

	if (!QuickSlot->IsValidIndex(InSlotIndex))
	{
		return false;
	}

	// 동일한 아이템을 동일한 슬롯에 넣고자 할때 체크
	if (QuickSlot->FindSlotIndex(InItemInstance) == InSlotIndex)
	{
		return false;
	}

	return true;
}

bool UQuickSlotComponent::CanRemoveFromQuickSlot(EQuickSlotType InSlotType, int32 InSlotIndex)
{
	FQuickSlot* QuickSlot = GetQuickSlot(InSlotType);
	if (QuickSlot == nullptr)
	{
		return false;
	}

	if (!QuickSlot->IsValidIndex(InSlotIndex))
	{
		return false;
	}

	if (!GetQuickSlotItem(InSlotType, InSlotIndex))
	{
		return false;
	}

	return true;
}

bool UQuickSlotComponent::SetActiveSlotNext(EQuickSlotType InSlotType)
{
	FQuickSlot* QuickSlot = GetQuickSlot(InSlotType);
	if (QuickSlot == nullptr)
	{
		return false;
	}
	
	int32 NextSlotIndex = (QuickSlot->ActiveSlotIndex + 1) % QuickSlot->GetSlotSize();
	SetActiveSlot(InSlotType, NextSlotIndex);
	return true;
}

bool UQuickSlotComponent::SetActiveSlotPrev(EQuickSlotType InSlotType)
{
	FQuickSlot* QuickSlot = GetQuickSlot(InSlotType);
	if (QuickSlot == nullptr)
	{
		return false;
	}

	int32 PrevSlotIndex = (QuickSlot->ActiveSlotIndex - 1 + QuickSlot->GetSlotSize()) % QuickSlot->GetSlotSize();
	SetActiveSlot(InSlotType, PrevSlotIndex);
	return true;
}

bool UQuickSlotComponent::SetActiveSlot(EQuickSlotType InSlotType, int32 InNewSlotIndex)
{
	FQuickSlot* QuickSlot = GetQuickSlot(InSlotType);
	if (QuickSlot == nullptr)
	{
		return false;
	}

	if (!QuickSlot->IsValidIndex(InNewSlotIndex))
	{
		return false;
	}

	if (QuickSlot->ActiveSlotIndex == InNewSlotIndex)
	{
		return false;
	}

	SetActiveSlot_Server(InSlotType, InNewSlotIndex);
	return true;
}

void UQuickSlotComponent::SetActiveSlot_Server_Implementation(EQuickSlotType InSlotType, int32 InNewSlotIndex)
{
	if (FQuickSlot* QuickSlot = GetQuickSlot(InSlotType))
	{
		TArray<FQuickSlot> PrevQuickSlotList = QuickSlotList;
		UItemInstance* OldItem = QuickSlot->Slots[QuickSlot->ActiveSlotIndex];
		QuickSlot->ActiveSlotIndex = InNewSlotIndex;

		ApplyActiveQuickSlotState(InSlotType, OldItem, QuickSlot->Slots[QuickSlot->ActiveSlotIndex]/* NewItem */);

		OnRep_QuickSlotList(PrevQuickSlotList);
	}
}

void UQuickSlotComponent::UnEquipWeapon()
{
	if (UEquipmentComponent* EquipmentComponent = GetEquipmentComponent())
	{
		if (EquippedInstance)
		{
			EquipmentComponent->UnequipItem(EquippedInstance);
			EquippedInstance = nullptr;
		}
	}
}

void UQuickSlotComponent::EquipWeapon()
{
	if (FQuickSlot* QuickSlot = GetQuickSlot(EQuickSlotType::Weapon))
	{
		if (UEquipmentComponent* EquipmentComponent = GetEquipmentComponent())
		{
			if (UItemInstance* ItemInstance = QuickSlot->Slots[QuickSlot->ActiveSlotIndex])
			{
				if (const UItemFragment_Equipment* EquipmentFragment = ItemInstance->FindItemFragmentByClass<UItemFragment_Equipment>())
				{
					EquippedInstance = EquipmentComponent->EquipItem(EquipmentFragment->EquipmentDefinition);
				}
			}
		}
	}
}

FQuickSlot* UQuickSlotComponent::GetQuickSlot(EQuickSlotType InSlotType)
{
	for (FQuickSlot& Element : QuickSlotList)
	{
		if (Element.SlotType == InSlotType)
			return &Element;
	}
	return nullptr;
}

UItemInstance* UQuickSlotComponent::GetQuickSlotItem(EQuickSlotType InSlotType, int32 InSlotIndex)
{
	FQuickSlot* QuickSlot = GetQuickSlot(InSlotType);
	if (QuickSlot == nullptr)
	{
		UE_LOG(LogQuickSlot, Error, TEXT("UQuickSlotComponent::GetQuickSlotItem - [%s] 퀵슬롯을 찾지 못했습니다."), *UCommonFunctionLibrary::GetEnumNameString<EQuickSlotType>(InSlotType));
		return nullptr;
	}

	if (QuickSlot->IsValidIndex(InSlotIndex) == false)
	{
		UE_LOG(LogQuickSlot, Error, TEXT("UQuickSlotComponent::GetQuickSlotItem - [%s] 퀵슬롯의 인덱스[%d]가 유효하지 않는 범위입니다."), *UCommonFunctionLibrary::GetEnumNameString<EQuickSlotType>(InSlotType), InSlotIndex);
		return nullptr;
	}

	return QuickSlot->Slots[InSlotIndex];
}

void UQuickSlotComponent::OnRep_QuickSlotList(TArray<FQuickSlot> OldQuickSlotList)
{
	if (QuickSlotList.Num() != OldQuickSlotList.Num())
		return;

	for (int32 i = 0; i < QuickSlotList.Num(); i++)
	{
		FQuickSlot& CurrentQuickSlot = QuickSlotList[i];
		FQuickSlot& OldQuickSlot = OldQuickSlotList[i];

		// 변경된 퀵슬롯 감지
		for (int32 j = 0; j < CurrentQuickSlot.Slots.Num(); j++)
		{
			if (CurrentQuickSlot.Slots[j] != OldQuickSlot.Slots[j])
			{
				OnQuickSlotChanged.Broadcast(CurrentQuickSlot.SlotType, j, CurrentQuickSlot.Slots[j]);
			}
		}

		if (CurrentQuickSlot.ActiveSlotIndex != OldQuickSlot.ActiveSlotIndex)
		{
			OnQuickSlotActiveIndexChanged.Broadcast(CurrentQuickSlot.SlotType, CurrentQuickSlot.ActiveSlotIndex);
		}
	}
}

UCombatAbilitySystemComponent* UQuickSlotComponent::GetAbilitySystemComponent() const
{
	AActor* OwningActor = GetOwner();
	return Cast<UCombatAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor));
}
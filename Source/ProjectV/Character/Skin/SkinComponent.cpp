// Fill out your copyright notice in the Description page of Project Settings.

#include "SkinComponent.h"
#include "SkinActor.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "ProjectVLogs.h"

#define SKIN_LOG(Verbosity, Format, ...) UE_LOG(LogSkin, Verbosity, TEXT("[SkinComponent] - %s"), *FString::Printf(Format, ##__VA_ARGS__))

void FSkinPartList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		FSkinPartEntry& Entry = Entries[Index];
		DestroySkinActor(Entry);
	}
}

void FSkinPartList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		FSkinPartEntry& Entry = Entries[Index];
		SpawnSkinActor(Entry);
	}
}

void FSkinPartList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	//for (int32 Index : ChangedIndices)
	//{
	//	SKIN_LOG(Log, TEXT("Skin Changed."));
	//}
}

void FSkinPartList::AddEntry(FGameplayTag TypeTag, TSubclassOf<ASkinActor> SkinActorClass, FName Socket)
{
	if (ensure(OwnerComponent) && OwnerComponent->GetOwner() && OwnerComponent->GetOwner()->HasAuthority())
	{
		FSkinPartEntry& NewEntry = Entries.AddDefaulted_GetRef();
		NewEntry.TypeTag = TypeTag;
		NewEntry.SkinActorClass = SkinActorClass;
		NewEntry.Socket = Socket;

		// *** Listen Server의 경우 복제가 호출되지 않으므로 직접 호출 ***
		SpawnSkinActor(NewEntry);

		MarkItemDirty(NewEntry);
	}
}

void FSkinPartList::RemoveEntry(FGameplayTag TypeTag)
{
	if (ensure(OwnerComponent) && OwnerComponent->GetOwner() && OwnerComponent->GetOwner()->HasAuthority())
	{
		for (auto It = Entries.CreateIterator(); It; ++It)
		{
			FSkinPartEntry& Entry = *It;
			if (Entry.TypeTag.MatchesTagExact(TypeTag))
			{
				DestroySkinActor(Entry);
				It.RemoveCurrent();
				MarkArrayDirty();
				break;
			}
		}
	}
}

FSkinPartEntry* FSkinPartList::GetEntry(FGameplayTag TypeTag)
{
	for (auto It = Entries.CreateIterator(); It; ++It)
	{
		FSkinPartEntry* Entry = &(*It);
		if (Entry->TypeTag.MatchesTagExact(TypeTag))
		{
			return Entry;
		}
	}
	return nullptr;
}

void FSkinPartList::SpawnSkinActor(FSkinPartEntry& Entry)
{
	if (ensure(OwnerComponent) && !OwnerComponent->IsNetMode(ENetMode::NM_DedicatedServer))
	{
		if (Entry.SkinActorClass != nullptr)
		{
			AActor* OwningActor = OwnerComponent->GetOwner();

			USceneComponent* AttachTo = OwningActor->GetRootComponent();
			if (ACharacter* OwningCharacter = Cast<ACharacter>(OwningActor))
			{
				AttachTo = OwningCharacter->GetMesh();
			}

			UChildActorComponent* ChildActorComponent = NewObject<UChildActorComponent>(OwningActor);
			ChildActorComponent->SetupAttachment(AttachTo, Entry.Socket);
			ChildActorComponent->SetChildActorClass(Entry.SkinActorClass);
			ChildActorComponent->RegisterComponent(); 

			/* RegisterComponent() -> OnRegister() 내부에서 ChildActor를 생성하므로 여기서부터 액터가 유효 */
			if (ASkinActor* SpawnedActor = Cast<ASkinActor>(ChildActorComponent->GetChildActor()))
			{
				SpawnedActor->SetActorEnableCollision(false);
			}

			Entry.SpawnedComponent = ChildActorComponent;
		}
	}
}

void FSkinPartList::DestroySkinActor(FSkinPartEntry& Entry)
{
	if (Entry.SpawnedComponent)
	{
		Entry.SpawnedComponent->DestroyComponent();
		Entry.SpawnedComponent = nullptr;
	}
}


USkinComponent::USkinComponent()
	: SkinList(this)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void USkinComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USkinComponent::OnRegister()
{
	Super::OnRegister();
}

void USkinComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void USkinComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, SkinList);
}

void USkinComponent::SetSkin(TSubclassOf<ASkinActor> SkinActorClass, FName Socket)
{
	if (GetOwner()->HasAuthority())
	{
		if (SkinActorClass == nullptr)
		{
			SKIN_LOG(Error, TEXT("SkinActorClass is nullptr."));
			return;
		}

		ASkinActor* SkinActorCDO = Cast<ASkinActor>(SkinActorClass->GetDefaultObject());
		FGameplayTag& SkinTypeTag = SkinActorCDO->GetTypeTag();
		if (HasSkin(SkinTypeTag))
		{
			SkinList.RemoveEntry(SkinTypeTag);
		}
		SkinList.AddEntry(SkinTypeTag, SkinActorClass, Socket);

		SKIN_LOG(Log, TEXT("Player(%s) Skin Changed. (Tag: %s)"), *GetOwner()->GetName(), *SkinTypeTag.ToString());
	}
}

bool USkinComponent::HasSkin(FGameplayTag TypeTag)
{
	return (SkinList.GetEntry(TypeTag) != nullptr);
}

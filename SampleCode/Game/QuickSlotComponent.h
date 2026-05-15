// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuickSlotComponent.generated.h"

class UCombatAbilitySystemComponent;
class UItemInstance;
class UInventoryComponent;
class UEquipmentInstance;
class UEquipmentComponent;

UENUM(BlueprintType)
enum class EQuickSlotType : uint8
{
	None,
	Weapon			UMETA(ToolTip = "주무기"),
	Consumption		UMETA(ToolTip = "소비 및 사용가능한 아이템"),
	Skill			UMETA(ToolTip = "단일 발동가능한 기술류"),
};

/* 초기 퀵슬롯을 생성하기 위해 사용되는 구조체입니다. */
USTRUCT(BlueprintType)
struct PROJECTV_API FRegisteredQuickSlotInfo
{
	GENERATED_BODY()

public:
	FRegisteredQuickSlotInfo();

public:
	/* 등록할 퀵슬롯 타입입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	EQuickSlotType SlotType;

	/* 퀵슬롯의 크기입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	int32 SlotSize;
};

/* 생성된 퀵슬롯 정보를 담고있는 구조체입니다. */
USTRUCT(BlueprintType)
struct PROJECTV_API FQuickSlot
{
	GENERATED_BODY()

public:
	FQuickSlot();

public:
	/* 슬롯이 수용 가능한 최대 크기를 반환합니다. */
	int32 GetSlotSize();

	/* 등록되지 않은 슬롯의 인덱스를 반환합니다. (없다면 -1을 반환합니다.) */
	int32 GetEmptySlotIndex();

	/* 슬롯에서 인덱스가 유효한지 검사합니다. */
	bool IsValidIndex(int32 Index);

	/* 슬롯 내 해당 아이템이 존재하는지 체크합니다. */
	bool HasItem(UItemInstance* ItemInstance);

	/* 슬롯 내 해당 아이템이 존재하는 슬롯의 인덱스를 반환합니다. (실패시 -1을 반환합니다.) */
	int32 FindSlotIndex(UItemInstance* ItemInstance);

public:
	UPROPERTY()
	EQuickSlotType SlotType;

	UPROPERTY()
	TArray<UItemInstance*> Slots;

	UPROPERTY()
	int32 ActiveSlotIndex;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuickSlotInitialized);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnQuickSlotChanged, EQuickSlotType, InSlotType, int32, InSlotIndex, UItemInstance*, InItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuickSlotActiveIndexChanged, EQuickSlotType, InSlotType, int32, InNewSlotIndex);

/*
*	퀵슬롯에 아이템을 등록하고 해제하는 기능을 담당하는 컴포넌트입니다.
*	아이템이 퀵슬롯에 등록되더라도 인벤토리에 유지됩니다.
*/
UCLASS(Blueprintable, BlueprintType, ClassGroup = (QuickSlot), meta = (BlueprintSpawnableComponent))
class PROJECTV_API UQuickSlotComponent : public UActorComponent
{
	GENERATED_BODY()

/*
*	#################################
*	#			Events				#
*	#################################
*/
public:
	UPROPERTY(BlueprintAssignable)
	FOnQuickSlotInitialized OnQuickSlotInitialized;

	UPROPERTY(BlueprintAssignable)
	FOnQuickSlotChanged OnQuickSlotChanged;

	UPROPERTY(BlueprintAssignable)
	FOnQuickSlotActiveIndexChanged OnQuickSlotActiveIndexChanged;

/*
*	#################################
*	#			Functions			#
*	#################################
*/
public:
	UQuickSlotComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	void CreateQuickSlots();
	void ApplyActiveQuickSlotState(EQuickSlotType InSlotType, UItemInstance* OldItem, UItemInstance* NewItem);

	UInventoryComponent* GetInventoryComponent();

	UEquipmentComponent* GetEquipmentComponent();

	UFUNCTION()
	void OnInventoryItemChanged(UItemInstance* ItemInstance, int32 OldCouint, int32 NewCount);

public:
	/* 퀵슬롯에 아이템을 등록합니다. */
	UFUNCTION(BlueprintCallable, Category = "QuickSlot")
	void AddToQuickSlot(UItemInstance* InItemInstance, EQuickSlotType InSlotType, int32 InSlotIndex);

	/* 퀵슬롯에서 입력받은 인덱스의 아이템을 등록 해제합니다.  */
	UFUNCTION(BlueprintCallable, Category = "QuickSlot")
	void RemoveFromQuickSlot(EQuickSlotType InSlotType, int32 InSlotIndex);

	/* 퀵슬롯을 비웁니다. */
	UFUNCTION(BlueprintCallable, Category = "QuickSlot")
	bool ClearQuickSlot(EQuickSlotType InSlotType);

private:
	UFUNCTION(Server, Reliable, Category = "QuickSlot")
	void AddToQuickSlot_Server(UItemInstance* InItemInstance, EQuickSlotType InSlotType, int32 InSlotIndex);
	void AddToQuickSlot_Server_Implementation(UItemInstance* InItemInstance, EQuickSlotType InSlotType, int32 InSlotIndex);

	UFUNCTION(Server, Reliable, Category = "QuickSlot")
	void RemoveFromQuickSlot_Server(EQuickSlotType InSlotType, int32 InSlotIndex);
	void RemoveFromQuickSlot_Server_Implementation(EQuickSlotType InSlotType, int32 InSlotIndex);

	UFUNCTION(Server, Reliable, Category = "QuickSlot")
	void ClearQuickSlot_Server(EQuickSlotType InSlotType);
	void ClearQuickSlot_Server_Implementation(EQuickSlotType InSlotType);

protected:
	bool CanAddToQuickSlot(UItemInstance* InItemInstance, EQuickSlotType InSlotType, int32 InSlotIndex);
	bool CanRemoveFromQuickSlot(EQuickSlotType InSlotType, int32 InSlotIndex);

public:
	/* 퀵슬롯의 활성화 슬롯을 다음으로 옮깁니다. */
	UFUNCTION(BlueprintCallable, Category = "QuickSlot")
	bool SetActiveSlotNext(EQuickSlotType InSlotType);

	/* 퀵슬롯의 활성화 슬롯을 이전으로 옮깁니다. */
	UFUNCTION(BlueprintCallable, Category = "QuickSlot")
	bool SetActiveSlotPrev(EQuickSlotType InSlotType);

	/* 퀵슬롯의 활성화 슬롯을 변경합니다. */
	UFUNCTION(BlueprintCallable, Category = "QuickSlot")
	bool SetActiveSlot(EQuickSlotType InSlotType, int32 InNewSlotIndex);

protected:
	UFUNCTION(Server, Reliable, Category = "QuickSlot")
	void SetActiveSlot_Server(EQuickSlotType InSlotType, int32 InNewSlotIndex);
	void SetActiveSlot_Server_Implementation(EQuickSlotType InSlotType, int32 InNewSlotIndex);

public:
	/* 현재 장착중인 Weapon 타입 아이템을 장착 해제합니다. */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "QuickSlot")
	void UnEquipWeapon();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "QuickSlot")
	void EquipWeapon();

	FQuickSlot* GetQuickSlot(EQuickSlotType InSlotType);

	UItemInstance* GetQuickSlotItem(EQuickSlotType InSlotType, int32 InSlotIndex);

protected:
	UFUNCTION()
	void OnRep_QuickSlotList(TArray<FQuickSlot> OldQuickSlotList);

private:
	UCombatAbilitySystemComponent* GetAbilitySystemComponent() const;

/*
*	#################################
*	#			Variables			#
*	#################################
*/
protected:
	UPROPERTY(EditAnywhere)
	TArray<FRegisteredQuickSlotInfo> RegisteredInfo;

	UPROPERTY(ReplicatedUsing = OnRep_QuickSlotList)
	TArray<FQuickSlot> QuickSlotList;

private:
	UPROPERTY()
	TObjectPtr<UEquipmentInstance> EquippedInstance;

	UPROPERTY()
	TObjectPtr<UInventoryComponent> CachedInventoryComponent;

	UPROPERTY()
	TObjectPtr<UEquipmentComponent> CachedEquipmentComponent;
};

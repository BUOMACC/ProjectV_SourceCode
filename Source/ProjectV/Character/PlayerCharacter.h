// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatCharacter.h"
#include "PlayerCharacter.generated.h"

class UProjectVUserSettingsShared;
class UPlayerComponent;
class UInventoryComponent;
class UEquipmentComponent;
class UQuickSlotComponent;
class USkinComponent;


/*
 *	Player가 조작 가능한 캐릭터의 베이스입니다.
 *	Input에 대한 바인딩이 포함되어 있습니다.
 */
UCLASS()
class PROJECTV_API APlayerCharacter : public ACombatCharacter
{
	GENERATED_BODY()
	
/*
	######################################
	#			  Functions				 #
	######################################
*/
public:
	APlayerCharacter();

	virtual void Tick(float DeltaSeconds) override;
	virtual void PreInitializeComponents() override;

	virtual void NotifyControllerChanged() override;
	
public:
	UPlayerComponent* GetPlayerComponent() const { return PlayerComponent; }
	UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BlockInputTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	void OnSharedSettingApplied(UProjectVUserSettingsShared* SharedSetting);

/*
	######################################
	#			  Variables				 #
	######################################
*/
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UPlayerComponent> PlayerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UInventoryComponent> InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UEquipmentComponent> EquipmentComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UQuickSlotComponent> QuickSlotComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkinComponent> SkinComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Setup")
	TSubclassOf<UAnimInstance> DefaultAnimLayerClass;

	/* 이 태그를 가진 어빌리티를 실행하는 동안, 락온에 의해 캐릭터가 회전되지 않습니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Setup")
	FGameplayTagContainer IgnoreLockonRotationTags;
};

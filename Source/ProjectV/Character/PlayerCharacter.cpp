// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Player/CombatPlayerController.h"
#include "CombatGameplayTags.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CombatSpringArmComponent.h"
#include "Character/PlayerComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Equipment/EquipmentComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/QuickSlotComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Skin/SkinComponent.h"
#include "Setting/ProjectVUserSettingsShared.h"

APlayerCharacter::APlayerCharacter()
{
	PlayerComponent = CreateDefaultSubobject<UPlayerComponent>(TEXT("PlayerComponent"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipmentComponent"));
	QuickSlotComponent = CreateDefaultSubobject<UQuickSlotComponent>(TEXT("QuickSlotComponent"));
	SkinComponent = CreateDefaultSubobject<USkinComponent>(TEXT("SkinComponent"));

	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 5.f;
	CameraBoom->CameraLagMaxDistance = 1000.f;

	CameraBoom->bEnableCameraRotationLag = true;
	CameraBoom->CameraRotationLagSpeed = 15.f;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Bind Events
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		const FGameplayTag& BlockInputTag = FCombatGameplayTags::Get().Character_State_BlockInput;
		ASC->RegisterGameplayTagEvent(BlockInputTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &APlayerCharacter::BlockInputTagChanged);
	}
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APlayerCharacter::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	if (DefaultAnimLayerClass)
	{
		GetMesh()->GetAnimInstance()->LinkAnimClassLayers(DefaultAnimLayerClass);
	}
}

void APlayerCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
	
	if (IsLocallyControlled())
	{
		if (ACombatPlayerController* CombatPC = Cast<ACombatPlayerController>(GetController()))
		{
			CombatPC->GetSharedSettings()->OnSettingApplied.AddUObject(this, &APlayerCharacter::OnSharedSettingApplied);
			OnSharedSettingApplied(CombatPC->GetSharedSettings());
		}
	}
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerComponent->BindInputs(PlayerInputComponent);
}

void APlayerCharacter::BlockInputTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	// 카운트가 있으면? -> BlockInput을 Add 했다는 의미이므로 InputBuffer를 비워 콤보를 진행하지 못하도록함
	if (NewCount > 0)
	{
		GetPlayerComponent()->ClearInputBuffer();
	}
}

void APlayerCharacter::OnSharedSettingApplied(UProjectVUserSettingsShared* SharedSetting)
{
	uint8 CameraFlag = SharedSetting->GetSmoothCamera() ? 1 : 0;
	CameraBoom->bEnableCameraRotationLag = CameraFlag;
}

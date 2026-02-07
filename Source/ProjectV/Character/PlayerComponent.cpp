// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerComponent.h"
#include "Player/CombatPlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/CombatAbilitySystemComponent.h"
#include "Character/CombatCharacter.h"
#include "CombatGameplayTags.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "Input/CombatEnhancedInputComponent.h"
#include "Inventory/QuickSlotComponent.h"
#include "ProjectVLogs.h"
#include "Setting/ProjectVUserSettingsShared.h"

UPlayerComponent::UPlayerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	InputBuffer = FGameplayTag::EmptyTag;
	bEnableInputBuffer = false;
}

void UPlayerComponent::BeginPlay()
{
	Super::BeginPlay();

	// Caching OwnerCharacter
	if (GetOwner() == nullptr)
	{
		UE_LOG(LogPlayerComponent, Error, TEXT("[PlayerComponent] OwnerCharacter is not set! (null pointer)"));
		return;
	}

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter == nullptr)
	{
		UE_LOG(LogPlayerComponent, Error, TEXT("[PlayerComponent] OwnerCharacter is not ACharacter! (cast failed)"));
		return;
	}

	// 캐릭터에 맞는 MappingContext 등록
	if (APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (DefaultMappingContext == nullptr)
			{
				UE_LOG(LogPlayerComponent, Error, TEXT("[PlayerComponent] DefaultMappingContext is nullptr"));
				return;
			}
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void UPlayerComponent::BindInputs(UInputComponent* PlayerInputComponent)
{
	if (InputConfig == nullptr)
	{
		UE_LOG(LogPlayerComponent, Error, TEXT("[PlayerComponent] InputConfig is nullptr!"));
		return;
	}

	if (UCombatEnhancedInputComponent* EnhancedInputComponent = CastChecked<UCombatEnhancedInputComponent>(PlayerInputComponent))
	{
		const FCombatGameplayTags& AllTags = FCombatGameplayTags::Get();

		// 자동 바인딩
		EnhancedInputComponent->BindAutoAction(InputConfig, this, &ThisClass::Input_AbilityPressed, &ThisClass::Input_AbilityReleased, &ThisClass::Input_AbilityStarted);

		// 수동 바인딩
		EnhancedInputComponent->BindManualAction(InputConfig, AllTags.Input_Type_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
		EnhancedInputComponent->BindManualAction(InputConfig, AllTags.Input_Type_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
		EnhancedInputComponent->BindManualAction(InputConfig, AllTags.Input_Type_LockonChange, ETriggerEvent::Triggered, this, &ThisClass::Input_LockonChange);

		EnhancedInputComponent->BindManualAction(InputConfig, AllTags.Input_Type_QuickSlot_ConsumptionPrev, ETriggerEvent::Triggered, this, &ThisClass::Input_QuickSlot_ConsumptionPrev);
		EnhancedInputComponent->BindManualAction(InputConfig, AllTags.Input_Type_QuickSlot_ConsumptionNext, ETriggerEvent::Triggered, this, &ThisClass::Input_QuickSlot_ConsumptionNext);
	
		EnhancedInputComponent->BindManualAction(InputConfig, AllTags.Input_Type_QuickSlot_SkillPrev, ETriggerEvent::Triggered, this, &ThisClass::Input_QuickSlot_SkillPrev);
		EnhancedInputComponent->BindManualAction(InputConfig, AllTags.Input_Type_QuickSlot_SkillNext, ETriggerEvent::Triggered, this, &ThisClass::Input_QuickSlot_SkillNext);
	}
}

/* ------------------ Input Buffer ------------------ */
void UPlayerComponent::EnableInputBuffer()
{
	bEnableInputBuffer = true;
}

void UPlayerComponent::DisableInputBuffer()
{
	bEnableInputBuffer = false;
}

void UPlayerComponent::ClearInputBuffer()
{
	InputBuffer = FGameplayTag::EmptyTag;
}

const FGameplayTag& UPlayerComponent::GetInputBuffer()
{
	return InputBuffer;
}
/* --------------------------------------------------- */

void UPlayerComponent::Input_Move(const FInputActionValue& InputValue)
{
	if (OwnerCharacter == nullptr)
		return;

	FVector2D MoveDir = InputValue.Get<FVector2D>();
	if (AController* Controller = OwnerCharacter->GetController())
	{
		const FRotator ControlRotation = Controller->GetControlRotation();
		const FRotator YawRot = FRotator(0.f, ControlRotation.Yaw, 0.f);
		
		// GetScaledAxis()에서 SafeNormal()한게 GetUnitAxis()
		const FVector ForwardDir = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
		const FVector RightDir = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);

		OwnerCharacter->AddMovementInput(ForwardDir, MoveDir.Y);
		OwnerCharacter->AddMovementInput(RightDir, MoveDir.X);
	}
}

void UPlayerComponent::Input_Look(const FInputActionValue& InputValue)
{
	if (OwnerCharacter == nullptr)
		return;

	FVector2D LookAxis = InputValue.Get<FVector2D>();
	if (AController* Controller = OwnerCharacter->GetController())
	{
		if (ACombatPlayerController* CombatPC = Cast<ACombatPlayerController>(Controller))
		{
			UProjectVUserSettingsShared* SharedSettings = UProjectVUserSettingsShared::LoadOrCreateSettings();
			LookAxis *= SharedSettings->GetMouseSensitivity();
		}

		OwnerCharacter->AddControllerYawInput(LookAxis.X);
		OwnerCharacter->AddControllerPitchInput(LookAxis.Y);
	}
}

void UPlayerComponent::Input_QuickSlot_ConsumptionPrev()
{
	if (OwnerCharacter == nullptr)
		return;

	if (UQuickSlotComponent* QuickSlotComponent = OwnerCharacter->FindComponentByClass<UQuickSlotComponent>())
	{
		QuickSlotComponent->SetActiveSlotPrev(EQuickSlotType::Consumption);
	}
}

void UPlayerComponent::Input_QuickSlot_ConsumptionNext()
{
	if (OwnerCharacter == nullptr)
		return;

	if (UQuickSlotComponent* QuickSlotComponent = OwnerCharacter->FindComponentByClass<UQuickSlotComponent>())
	{
		QuickSlotComponent->SetActiveSlotNext(EQuickSlotType::Consumption);
	}
}

void UPlayerComponent::Input_QuickSlot_SkillPrev()
{
	if (OwnerCharacter == nullptr)
		return;

	if (UQuickSlotComponent* QuickSlotComponent = OwnerCharacter->FindComponentByClass<UQuickSlotComponent>())
	{
		QuickSlotComponent->SetActiveSlotPrev(EQuickSlotType::Skill);
	}
}

void UPlayerComponent::Input_QuickSlot_SkillNext()
{
	if (OwnerCharacter == nullptr)
		return;

	if (UQuickSlotComponent* QuickSlotComponent = OwnerCharacter->FindComponentByClass<UQuickSlotComponent>())
	{
		QuickSlotComponent->SetActiveSlotNext(EQuickSlotType::Skill);
	}
}

void UPlayerComponent::Input_LockonChange(const FInputActionValue& InputValue)
{
	if (OwnerCharacter == nullptr)
		return;


	FGameplayEventData Payload;
	Payload.EventMagnitude = InputValue.Get<float>();

	float AxisValue = InputValue.Get<float>();
	if (AxisValue < 0.f)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), FCombatGameplayTags::Get().Input_Type_LockonChange_Left, Payload);
	}
	else if (AxisValue > 0.f)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), FCombatGameplayTags::Get().Input_Type_LockonChange_Right, Payload);
	}
}

// Run on Ability
//void UPlayerComponent::Input_Jump(const FInputActionValue& InputValue)
//{
//	if (OwnerCharacter == nullptr)
//		return;
//
//	OwnerCharacter->Jump();
//}
//
//void UPlayerComponent::Input_JumpStop(const FInputActionValue& InputValue)
//{
//	if (OwnerCharacter == nullptr)
//		return;
//
//	OwnerCharacter->StopJumping();
//}

void UPlayerComponent::Input_AbilityPressed(FGameplayTag AbilityInputTag)
{
	if (OwnerCharacter == nullptr)
		return;

	if (ACombatCharacter* CombatCharacter = Cast<ACombatCharacter>(OwnerCharacter))
	{
		if (UCombatAbilitySystemComponent* CombatASC = CombatCharacter->GetCombatAbilitySystemComponent())
		{
			CombatASC->AbilityInputPressedTag(AbilityInputTag);
		}
	}
}

void UPlayerComponent::Input_AbilityReleased(FGameplayTag AbilityInputTag)
{
	if (OwnerCharacter == nullptr)
		return;

	if (ACombatCharacter* CombatCharacter = Cast<ACombatCharacter>(OwnerCharacter))
	{
		if (UCombatAbilitySystemComponent* CombatASC = CombatCharacter->GetCombatAbilitySystemComponent())
		{
			CombatASC->AbilityInputReleasedTag(AbilityInputTag);
		}
	}
}

void UPlayerComponent::Input_AbilityStarted(FGameplayTag AbilityInputTag)
{
	if (OwnerCharacter == nullptr)
		return;

	if (ACombatCharacter* CombatCharacter = Cast<ACombatCharacter>(OwnerCharacter))
	{
		// InputBuffer에 입력 등록
		if (bEnableInputBuffer)
		{
			InputBuffer = AbilityInputTag;
		}
	}
}
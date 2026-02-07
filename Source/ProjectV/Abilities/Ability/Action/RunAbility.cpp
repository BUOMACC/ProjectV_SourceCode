// Fill out your copyright notice in the Description page of Project Settings.


#include "RunAbility.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "GameFramework/CharacterMovementComponent.h"

FRunMovementData::FRunMovementData()
{
	Reset();
}

void FRunMovementData::Fill(UCharacterMovementComponent* InCharacterMovement)
{
	if (InCharacterMovement)
	{
		Speed = InCharacterMovement->MaxWalkSpeed;
		Deceleration = InCharacterMovement->BrakingDecelerationWalking;
		Acceleration = InCharacterMovement->MaxAcceleration;
		RotationRate = InCharacterMovement->RotationRate;
	}
}

void FRunMovementData::Reset()
{
	Speed = 0.f;
	Acceleration = 0.f;
	Deceleration = 0.f;
	RotationRate = FRotator::ZeroRotator;
}

URunAbility::URunAbility()
{
	// Tags
	AbilityTags.AddTag(FCombatGameplayTags::Get().Ability_Type_Movement_Run);

	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Character_State_Run);

	ActivationBlockedTags.AddTag(FCombatGameplayTags::Get().Character_MainState_Airborne);
	ActivationBlockedTags.AddTag(FCombatGameplayTags::Get().Character_MainState_Ground);
	ActivationBlockedTags.AddTag(FCombatGameplayTags::Get().Character_MainState_Dead);
	ActivationBlockedTags.AddTag(FCombatGameplayTags::Get().Character_MainState_Hit);
	ActivationBlockedTags.AddTag(FCombatGameplayTags::Get().Character_State_Attack);
	ActivationBlockedTags.AddTag(FCombatGameplayTags::Get().Character_State_Guard);
	ActivationBlockedTags.AddTag(FCombatGameplayTags::Get().Character_State_Dodge);
	ActivationBlockedTags.AddTag(FCombatGameplayTags::Get().Character_State_Skill);

	// Default Value Run Data
	RunData.Speed = 600.f;
	RunData.Acceleration = 1200.f;
	RunData.Deceleration = 950.f;
	RunData.RotationRate = FRotator(0.f, 540.f, 0.f);

	// Inputs
	InputTag = FCombatGameplayTags::Get().Input_Type_Run;
	bAutoTriggerWhenInput = true;
}

void URunAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UCharacterMovementComponent* CharacterMovement = GetCharacterMovementComponent();
	check(CharacterMovement);

	// 1) 기존 데이터 캐싱
	RestoreData.Fill(CharacterMovement);

	// 2) 설정한 달리기 데이터 적용
	CharacterMovement->MaxWalkSpeed = RunData.Speed;
	CharacterMovement->BrakingDecelerationWalking = RunData.Deceleration;
	CharacterMovement->MaxAcceleration = RunData.Acceleration;
	CharacterMovement->RotationRate = RunData.RotationRate;

	// 3) 키입력 태스크 바인딩
	UAbilityTask_WaitInputRelease* Task = UAbilityTask_WaitInputRelease::WaitInputRelease(this, false);
	Task->OnRelease.AddDynamic(this, &URunAbility::OnInputRelease);

	Task->ReadyForActivation();
}

bool URunAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void URunAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UCharacterMovementComponent* CharacterMovement = GetCharacterMovementComponent();
	check(CharacterMovement);

	CharacterMovement->MaxWalkSpeed = RestoreData.Speed;
	CharacterMovement->BrakingDecelerationWalking = RestoreData.Deceleration;
	CharacterMovement->MaxAcceleration = RestoreData.Acceleration;
	CharacterMovement->RotationRate = RestoreData.RotationRate;

	// clean up data
	RestoreData.Reset();

	ApplyCooldown(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URunAbility::OnInputRelease(float TimeHeld)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

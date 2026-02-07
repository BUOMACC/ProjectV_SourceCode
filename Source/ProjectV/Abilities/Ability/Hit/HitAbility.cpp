// Fill out your copyright notice in the Description page of Project Settings.


#include "HitAbility.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FunctionLibrary/CommonFunctionLibrary.h"
#include "FunctionLibrary/CombatAbilityFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"

UHitAbility::UHitAbility()
{
	AbilityTags.AddTag(FCombatGameplayTags::Get().ActivationType_Active);

	CancelAbilitiesWithTag.AddTag(FCombatGameplayTags::Get().Ability_Type_Action_Guard);
	//CancelAbilitiesWithTag.AddTag(FCombatGameplayTags::Get().Ability_Type_Movement_Crouch);

	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Character_State_BlockMovement);
	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Input_State_DisableFirstCombo);

	bRetriggerInstancedAbility = true;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ServerOnlyExecution;

	// Set Variable Default
	Attacker = nullptr;
	HitData = nullptr;
	HitAngle = 0.f;
}

void UHitAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, CurrentActivationInfo, TriggerEventData);

	// Apply Push
	UCharacterMovementComponent* MovementComp = GetCombatCharacter()->GetCharacterMovement();
	MovementComp->StopMovementImmediately();

	if (GetHitData())
	{
		FVector PushDirection = FVector::ZeroVector;

		// Guard, Parry에 적용되는 넉백값은 다르게 계산되어야 합니다.
		FGameplayTagContainer TagContainer;
		TagContainer.AddTag(FCombatGameplayTags::Get().Character_State_Guard);
		TagContainer.AddTag(FCombatGameplayTags::Get().Character_State_Parry);

		UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
		if (ASC->HasAnyMatchingGameplayTags(TagContainer))
		{
			PushDirection = GetHitData()->GetGuardPushDirection(GetAttacker(), GetCombatCharacter());
		}
		else
		{
			PushDirection = GetHitData()->GetPushDirection(GetAttacker(), GetCombatCharacter());
		}
		
		float PushStrength = PushDirection.Length();
		float PushTime = 0.2f;
		if (PushStrength > 600.f)
		{
			PushTime = (PushDirection.Length() / 600.f) * 0.2f;
		}
		ApplyPush(PushDirection.GetSafeNormal(), PushStrength, PushTime);
	}

	GetCombatCharacter()->UnCrouch();
}

bool UHitAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags);
}

void UHitAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	Attacker = nullptr;
	HitData = nullptr;
	HitAngle = 0.f;
}

ACombatCharacter* UHitAbility::GetAttacker()
{
	if (CurrentEventData.ContextHandle.IsValid())
	{
		const FGameplayEffectContextHandle& EffectContextHandle = CurrentEventData.ContextHandle;
		Attacker = Cast<ACombatCharacter>(EffectContextHandle.GetEffectCauser());
	}

	return Attacker;
}

UHitData* UHitAbility::GetHitData()
{
	if (CurrentEventData.ContextHandle.IsValid())
	{
		const FGameplayEffectContextHandle& EffectContextHandle = CurrentEventData.ContextHandle;
		HitData = UCombatAbilityFunctionLibrary::EffectContextGetHitData(CurrentEventData.ContextHandle);
	}

	return HitData;
}

float UHitAbility::GetHitAngle()
{
	if (GetAttacker() == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("UHitAbility::GetHitAngle -> Attacker is nullptr. return to zero. [%s]"), *GetName());
		return 0.f;
	}

	if (GetCombatCharacter() == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("UHitAbility::GetHitAngle -> Current AvatarActor is nullptr. return to zero. [%s]"), *GetName());
		return 0.f;
	}

	// Set Hit Angle
	HitAngle = UCommonFunctionLibrary::GetTwoActorAngle(GetCombatCharacter(), GetAttacker());
	HitAngle = UKismetMathLibrary::ClampAxis(HitAngle);	// [-180 ~ 180] to [0 ~ 360]

	return HitAngle;
}

EHitDirectionType UHitAbility::GetHitDirectionType()
{
	float Angle = GetHitAngle();

	if (Angle >= 315.f || Angle < 45.f)
		return EHitDirectionType::Forward;
	else if (Angle >= 45.f && Angle < 135.f)
		return EHitDirectionType::Right;
	else if (Angle >= 135.f && Angle < 225.f)
		return EHitDirectionType::Backward;
	else 
		return EHitDirectionType::Left;

	// default case (위에서 다 처리 되겠지만... 반환은 해야 하니까)
	return EHitDirectionType::Forward;
}

void UHitAbility::ApplyPush(FVector KnockbackLocation, float Strength, float Duration)
{
	UAbilityTask_ApplyRootMotionConstantForce* Task = UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(this, TEXT("Knockback"), KnockbackLocation, Strength, Duration, false, KnockbackCurve, ERootMotionFinishVelocityMode::ClampVelocity, FVector::ZeroVector, 300.f, false);
	Task->ReadyForActivation();
	//GetCharacterMovementComponent()->Launch(KnockbackLocation * Strength);
}

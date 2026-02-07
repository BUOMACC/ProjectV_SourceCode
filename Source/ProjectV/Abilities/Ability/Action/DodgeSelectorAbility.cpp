// Fill out your copyright notice in the Description page of Project Settings.


#include "DodgeSelectorAbility.h"
#include "Character/CombatCharacter.h"
#include "CombatMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UDodgeSelectorAbility::UDodgeSelectorAbility()
{
	// 분리된 단순 로직이기 때문에, 별도의 어빌리티 자체 태그는 필요하지 않고 입력에만 반응하면 됩니다.
	InputTag = FCombatGameplayTags::Get().Input_Type_Dodge;
	bAutoTriggerWhenInput = true;

	// 로컬 클라이언트에서만 로직을 수행합니다.
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalOnly;
}

void UDodgeSelectorAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	check(ASC);

	if (TSubclassOf<UGameplayAbility> DodgeAbility = SelectDodgeAbility())
	{
		if (IsValid(DodgeAbility))
		{
			ASC->TryActivateAbilityByClass(DodgeAbility);
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

bool UDodgeSelectorAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	return bResult && GetCombatCharacter()->GetCombatMovementComponent()->CanUnCrouch();
}

void UDodgeSelectorAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

TSubclassOf<UGameplayAbility> UDodgeSelectorAbility::SelectDodgeAbility()
{
	UCharacterMovementComponent* CharacterMovement = GetCharacterMovementComponent();
	check(CharacterMovement);

	EDirection Direction = ConvertAccelerationToDirection(CharacterMovement->GetCurrentAcceleration());
	return DirectionalDodgeAbility[(int32)Direction];
}

EDirection UDodgeSelectorAbility::ConvertAccelerationToDirection(const FVector& InAcceleration)
{
	// 1) 가속중이지 않다면, 기본적으로 뒤를 반환합니다.
	FVector NormalizedAcceleration2D = InAcceleration.GetSafeNormal2D();
	if (NormalizedAcceleration2D == FVector::ZeroVector)
	{
		return EDirection::Backward;
	}

	// 2) 락온상태가 아니라면, 항상 정면을 반환합니다.
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	check(ASC);
	if (!ASC->HasMatchingGameplayTag(FCombatGameplayTags::Get().Character_State_Lockon))
	{
		return EDirection::Forward;
	}

	// 3) 락온상태에서 가속중에는 각도에 따라 방향을 선택합니다.
	FRotator CameraRotation = GetActorController()->GetControlRotation();
	CameraRotation.Roll = 0.f;
	CameraRotation.Pitch = 0.f;

	FVector CameraForward = CameraRotation.Vector();
	float Dot = FVector::DotProduct(CameraForward, NormalizedAcceleration2D);
	float Angle = FMath::RadiansToDegrees(FMath::Acos(Dot));

	FVector Cross = FVector::CrossProduct(GetAvatarActorFromActorInfo()->GetActorForwardVector(), NormalizedAcceleration2D);
	if (Cross.Z < 0)
		Angle *= -1;

	// [-180 ~ 180] to [0 ~ 360]
	Angle = UKismetMathLibrary::ClampAxis(Angle);

	if (UKismetMathLibrary::InRange_FloatFloat(Angle, 345.f, 360.f, true, false) || UKismetMathLibrary::InRange_FloatFloat(Angle, 0.f, 15.f, true, false))
	{
		return EDirection::Forward;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(Angle, 15.f, 75.f, true, false))
	{
		return EDirection::ForwardRight;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(Angle, 75.f, 105.f, true, false))
	{
		return EDirection::Right;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(Angle, 105.f, 165.f, true, false))
	{
		return EDirection::BackwardRight;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(Angle, 165.f, 195.f, true, false))
	{
		return EDirection::Backward;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(Angle, 195.f, 255.f, true, false))
	{
		return EDirection::BackwardLeft;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(Angle, 255.f, 285.f, true, false))
	{
		return EDirection::Left;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(Angle, 285.f, 345.f, true, false))
	{
		return EDirection::ForwardLeft;
	}

	// 위에서 모든 경우의수를 처리한다고 하더라도, 반환은 해야하기 때문에...
	return EDirection::Forward;
}

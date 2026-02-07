// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Ability/ProjectVGameplayAbility.h"
#include "RunAbility.generated.h"

USTRUCT(BlueprintType)
struct PROJECTV_API FRunMovementData
{
	GENERATED_BODY()

public:
	FRunMovementData();

	/* 입력받은 CharacterMovementComponent 데이터를 이용해 변수를 채워넣습니다. */
	void Fill(UCharacterMovementComponent* InCharacterMovement);

	/* 구조체를 초기화합니다. */
	void Reset();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Speed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Acceleration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Deceleration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FRotator RotationRate;
};

/**
 *	달리기 상태로 전환하는 어빌리티입니다.
 */
UCLASS()
class PROJECTV_API URunAbility : public UProjectVGameplayAbility
{
	GENERATED_BODY()
	
public:
	URunAbility();

	// UGameplayAbility
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	// ~UGameplayAbility

protected:
	UFUNCTION()
	void OnInputRelease(float TimeHeld);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Run Ability")
	FRunMovementData RunData;

private:
	FRunMovementData RestoreData;
};

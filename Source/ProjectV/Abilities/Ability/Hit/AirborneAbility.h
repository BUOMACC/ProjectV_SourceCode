// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitAbility.h"
#include "AirborneAbility.generated.h"

/**
 *	공중으로 띄우는 피격 반응에 대한 어빌리티입니다.
 *	공중으로 뜨는 시점부터 지면에 닿는 순간까지 적용됩니다.
 * 
 *	 ※ 쓰러진 이후 일어나는 어빌리티인 Ground Ability와 연계하세요.
 */
UCLASS()
class PROJECTV_API UAirborneAbility : public UHitAbility
{
	GENERATED_BODY()
	
public:
	UAirborneAbility();

protected:
	// UGameplayAbility
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	// ~UGameplayAbility

private:
	void OnDelay();

	UFUNCTION()
	void OnMovementModeChanged(EMovementMode NewMovementMode);

	void PlayAirborneEndMontage();

private:
	/* Airborne End */
	UFUNCTION()
	void OnAirborneEndCompleted();

	UFUNCTION()
	void OnAirborneEndInterrupted();

	UFUNCTION()
	void OnAirborneEndCancelled();

protected:
	/* 공중에 솟아오르고 유지되는 상태에 대한 애니메이션입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Airborne Ability")
	TObjectPtr<UAnimMontage> Airborne_Start;

	/* 지면에 닿는 순간에 대한 애니메이션입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Airborne Ability")
	TObjectPtr<UAnimMontage> Airborne_End;

	/* 최소 해당시간 이후에 지면에 닿았는지 체크합니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Airborne Ability")
	float GroundDetectDelay;

	/* Airborne_Start 및 Airborne_End 몽타주의 재생 속도입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Airborne Ability")
	float PlayRate;

private:
	FTimerHandle FirstDelayTimerHandle;
};

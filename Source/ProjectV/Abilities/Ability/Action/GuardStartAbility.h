// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Ability/ActionAbility.h"
#include "GuardStartAbility.generated.h"

/**
 *	가드 시작시 실행되는 어빌리티입니다.
 *	키 입력을 해제하거나 가드시작 애니메이션 재생이 종료되면, 가드가 해제됩니다.
 * 
 *	 < 가드를 자동으로 해제하지 않고 유지하는 방법 >
 *	   가드를 올리는 동작이 종료되더라도 가드를 유지하려면 아래 방식대로 설정하세요.
 *	     1) Guard Start AnimMontage 내에서 종료되는 부분에 Branch NotifyState를 추가합니다.
 *	     2) 추가한 Branch NotifyState의 Action을 Activate Ability로 설정합니다.
 *	     3) 실행해야 하는 AbilityClass를 GuardHoldAbility로 지정합니다.
 */
UCLASS()
class PROJECTV_API UGuardStartAbility : public UProjectVGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGuardStartAbility();

protected:
	// UGameplayAbility
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	// ~UGameplayAbility

	UFUNCTION()
	void OnInputRelease(float TimeHeld);

	/* Guard Strart */
	UFUNCTION()
	void OnGuardStartMontageCompleted();

	UFUNCTION()
	void OnGuardStartMontageInterrupted();

	UFUNCTION()
	void OnGuardStartMontageCanceled();

	/* Guard End */
	UFUNCTION()
	void OnGuardStopMontageCompleted();

	UFUNCTION()
	void OnGuardStopMontageInterrupted();

	UFUNCTION()
	void OnGuardStopMontageCacncelled();

protected:
	/* 가드를 올릴때 실행되는 애니메이션입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Guard Start Ability")
	TObjectPtr<UAnimMontage> GuardStartMontage;

	/* 가드를 내릴때 실행되는 애니메이션입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Guard Start Ability")
	TObjectPtr<UAnimMontage> GuardStopMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Guard Start Ability")
	float PlayRate;

	/* 가드를 올릴때 적용되는 이펙트입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Guard Start Ability")
	TSubclassOf<UGameplayEffect> GameplayEffectToApply;
};

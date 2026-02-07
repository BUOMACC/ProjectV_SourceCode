// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Ability/ProjectVGameplayAbility.h"
#include "Net/UnrealNetwork.h"
#include "DeathBlowTargetAbility.generated.h"

/**
 *	치명적 일격(인살)을 가하는 실행자의 어빌리티입니다.
 */
UCLASS()
class PROJECTV_API UDeathBlowTargetAbility : public UProjectVGameplayAbility
{
	GENERATED_BODY()
	
public:
	UDeathBlowTargetAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageCanceled();

	UFUNCTION()
	void OnReceiveDeathBlowEvent(FGameplayEventData Payload);

	UFUNCTION()
	void OnReceiveDeathBlowRestoreEvent(FGameplayEventData Payload);


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DeathBlow Target Ability")
	TObjectPtr<UAnimMontage> DeathBlowTargetMontage;

	/* DeathBlow Notify 이벤트가 발생했을 때 적용되는 이펙트입니다. (ex: 체력 및 라이프를 차감하는 이펙트) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DeathBlow Target Ability")
	TSubclassOf<UGameplayEffect> DeathBlowEffect;

	/* DeathBlowRestore Notify 이벤트가 발생했을 때 적용되는 이펙트입니다. (ex: 라이프가 남아있을 때 체력을 다시 회복하는 이펙트) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DeathBlow Target Ability")
	TSubclassOf<UGameplayEffect> DeathBlowRestoreEffect;

private:
	TWeakObjectPtr<AActor> Instigator;
};

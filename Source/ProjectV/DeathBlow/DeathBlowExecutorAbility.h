// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Ability/ProjectVGameplayAbility.h"
#include "DeathBlowExecutorAbility.generated.h"

class UInputMappingContext;

/**
 *	치명적 일격(인살)을 가하는 실행자의 어빌리티입니다.
 */
UCLASS()
class PROJECTV_API UDeathBlowExecutorAbility : public UProjectVGameplayAbility
{
	GENERATED_BODY()
	
public:
	UDeathBlowExecutorAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION()
	void OnNetSync();

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageCanceled();

private:
	void FindBestLocationAndRotation(AActor* Target, FVector& OutLocation, FRotator& OutRotation) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DeathBlow Executor Ability")
	TObjectPtr<UAnimMontage> DeathBlowMontage;

	/* 어빌리티가 부여되면 적용될 인풋 매핑 컨텍스트입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DeathBlow Executor Ability")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	/* 사용될 모션워핑 이름입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DeathBlow Executor Ability")
	FName WarpingName;

	/* 워핑될 X값의 차이입니다. (ex: 100입력시 타겟의 앞 방향을 기준으로 100만큼 떨어진 위치입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DeathBlow Executor Ability")
	float WarpDistance;

	/* 대상이 실행해야 할 피격 태그입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DeathBlow Executor Ability", meta = (Categories = "Ability.Type.Hit.DeathBlow"))
	FGameplayTag DeathBlowTargetTag;

private:
	TWeakObjectPtr<AActor> TargetActor;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Ability/ProjectVGameplayAbility.h"
#include "SitAbility.generated.h"

class UCommonActivatableWidget;

/**
 *	상호작용(앉기)에 대한 어빌리티입니다.
 */
UCLASS()
class PROJECTV_API USitAbility : public UProjectVGameplayAbility
{
	GENERATED_BODY()

public:
	USitAbility();

protected:
	// UGameplayAbility
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	// ~UGameplayAbility

	UFUNCTION()
	void OnNetSync();

private:
	void CallProcessExitSitting();

	void ProcessExitSitting();

	UFUNCTION(Server, Reliable)
	void ProcessExitSitting_Server();

protected:
	UFUNCTION()
	void OnSitExitMontageInterrupted();

	UFUNCTION()
	void OnSitExitMontageCompleted();

	UFUNCTION()
	void OnSitExitMontageCanceled();

private:
	void OnDelay();

protected:
	/* 사용될 모션워핑 이름입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sit Ability")
	FName WarpingName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sit Ability")
	TObjectPtr<UAnimMontage> SitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sit Ability")
	TObjectPtr<UAnimMontage> SitExitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sit Ability")
	float PlayRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sit Ability|Widget")
	TSubclassOf<UCommonActivatableWidget> WidgetToOpen;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sit Ability|Widget")
	float WidgetOpenDelay;

private:
	FTimerHandle TimerHandle;
};

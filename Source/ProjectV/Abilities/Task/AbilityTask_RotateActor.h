// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_RotateActor.generated.h"

UENUM(BlueprintType)
enum class ERotateTaskRule : uint8
{
	/* 회전 없음 */
	None,

	/* 입력 방향으로 회전 */
	InputVector,

	/* 첫 카메라 방향으로 회전 */
	Camera,

	/* 시간동안 카메라 방향으로 계속해서 회전 */
	TrackCamera,
};

class ACharacter;
class UCombatMovementComponent;

/**
 *	Task 실행자를 입력한 시간동안 목표에 도달하도록 회전시키는 기능
 */
UCLASS()
class PROJECTV_API UAbilityTask_RotateActor : public UAbilityTask
{
	GENERATED_BODY()

public:
	UAbilityTask_RotateActor();

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_RotateActor* RotateActor(UGameplayAbility* OwningAbility, FName TaskInstanceName, ERotateTaskRule RotateTaskRule, float RotateSpeed = 800.f, float Duration = 0.25f, bool bIgnoreLockon = false);

protected:
	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
	virtual void OnDestroy(bool AbilityIsEnding) override;

	void GetTargetRotationByRotateTaskRule(ERotateTaskRule Rule, ACharacter* Character, FRotator& OutTargetRotation);

private:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRotateFinished);
	UPROPERTY(BlueprintAssignable)
	FOnRotateFinished OnFinish;

	ERotateTaskRule RotateTaskRule;

	bool bIsFinished;
	bool bIgnoreLockon;
	
	float RotateSpeed;
	float Duration;
	float RemainDuration;

	FRotator StartRot;
	FRotator TargetRot;

	UPROPERTY()
	TObjectPtr<UCombatMovementComponent> CachedCombatMovementComponent;
};

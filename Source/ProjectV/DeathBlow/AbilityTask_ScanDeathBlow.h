// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "UObject/ScriptInterface.h"
#include "AbilityTask_ScanDeathBlow.generated.h"


/**
 *	Task 실행자를 기준으로 주변 대상이 치명적 일격(인살) 가능한지 판단하는 태스크입니다.
 *  스캔되고 치명적 일격(인살)이 가능한 경우 관련 어빌리티를 부여합니다.
 */
UCLASS()
class PROJECTV_API UAbilityTask_ScanDeathBlow : public UAbilityTask
{
	GENERATED_BODY()

public:
	UAbilityTask_ScanDeathBlow();

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_ScanDeathBlow* ScanDeathBlow(UGameplayAbility* OwningAbility, float InDeathBlowScanRate, float InDeathBlowScanRadius, TEnumAsByte<ETraceTypeQuery> InTraceChannel);

protected:
	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityIsEnding) override;

	void QueryDeathBlowActors();

private:
	TEnumAsByte<ECollisionChannel> CollisionChannel;

	float DeathBlowScanRate = 0.1f;
	float DeathBlowScanRadius = 300.f;

	FTimerHandle TimerHandle;

	FGameplayAbilitySpecHandle GrantedAbilityHandle;
};

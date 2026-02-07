// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitVelocityBelowThreshold.generated.h"

class UMovementComponent;

UCLASS()
class PROJECTV_API UAbilityTask_WaitVelocityBelowThreshold : public UAbilityTask
{
	GENERATED_BODY()

public:
	UAbilityTask_WaitVelocityBelowThreshold(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "WaitVelocityBelowThreshold", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_WaitVelocityBelowThreshold* WaitVelocityBelowThreshold(UGameplayAbility* OwningAbility, bool bIgnoreZ, float VelocityThreshold);

protected:
	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
	virtual void OnDestroy(bool AbilityIsEnding) override;

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWaitVelocityBelowThresholdDelegate);
	UPROPERTY(BlueprintAssignable)
	FWaitVelocityBelowThresholdDelegate OnVelocityBelowThreshold;

protected:
	UPROPERTY()
	TObjectPtr<UMovementComponent>	CachedMovementComponent;

	bool bIgnoreZ;
	float VelocityThreshold;
};

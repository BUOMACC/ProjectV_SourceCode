// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitStartAccelerate.generated.h"

class UCharacterMovementComponent;

UCLASS()
class PROJECTV_API UAbilityTask_WaitStartAccelerate : public UAbilityTask
{
	GENERATED_BODY()

public:
	UAbilityTask_WaitStartAccelerate(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "WaitStartAccelerate", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_WaitStartAccelerate* WaitStartAccelerate(UGameplayAbility* OwningAbility, bool bCheckOnActivate);

protected:
	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
	virtual void OnDestroy(bool AbilityIsEnding) override;

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWaitStartAccelerateDelegate);
	UPROPERTY(BlueprintAssignable)
	FWaitStartAccelerateDelegate OnStartAccelerate;

protected:
	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> CachedMovementComponent;

	bool bCheckOnActivate;
};

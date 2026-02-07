// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "UObject/ScriptInterface.h"
#include "AbilityTask_Tick.generated.h"

class IInteractionInterface;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTick, float, DeltaTime);

/**
 *	매 틱마다 호출되는 로직을 위한 태스크입니다.
 */
UCLASS()
class PROJECTV_API UAbilityTask_Tick : public UAbilityTask
{
	GENERATED_BODY()

public:
	UAbilityTask_Tick();

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_Tick* Tick(UGameplayAbility* OwningAbility);

public:
	UPROPERTY(BlueprintAssignable)
	FOnTick OnTick;

protected:
	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
};

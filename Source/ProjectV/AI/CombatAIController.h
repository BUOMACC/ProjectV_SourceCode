// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GenericTeamAgentInterface.h"
#include "ProjectVCommonEnums.h"
#include "CombatAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
class UAISenseConfig_Damage;

UCLASS()
class PROJECTV_API ACombatAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ACombatAIController();

	virtual void Tick(float DeltaTime) override;

public:
	// Team Agent Interface
	virtual void SetGenericTeamId(const FGenericTeamId& InTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	// ~Team Agent Interface

public:
	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	bool CanSenseActor(AActor* Actor, EAISenseType SenseType, FAIStimulus& OutStimulus);

	/* 시야에서 타겟을 감지한 경우 호출됩니다. */
	void HandleSenseSight(AActor* Actor, const FAIStimulus& Stimulus);

	/* 시야로부터 타겟이 벗어난 경우 호출됩니다. */
	void HandleEndSenseSight();

	/* 시야에서 타겟을 감지한 경우 호출됩니다. (BP) */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void HandleSenseSightBP(AActor* Actor, const FAIStimulus& Stimulus);

	/* 시야로부터 타겟이 벗어난 경우 호출됩니다. (BP) */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void HandleEndSenseSightBP();


	/* 주변으로 부터 소리를 들으면 호출됩니다. */
	void HandleSenseHearing(AActor* Actor, const FAIStimulus& Stimulus);

	/* 주변으로 부터 소리를 들으면 호출됩니다. (BP) */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void HandleSenseHearingBP(AActor* Actor, const FAIStimulus& Stimulus);


	/* 피해를 받으면 호출됩니다. */
	void HandleSenseDamaged(AActor* Actor, const FAIStimulus& Stimulus);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void HandleSenseDamagedBP(AActor* Actor, const FAIStimulus& Stimulus);

private:
	bool HasTarget() const;

protected:
	UPROPERTY()
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	UPROPERTY()
	TObjectPtr<UAISenseConfig_Hearing> HearingConfig;

	UPROPERTY()
	TObjectPtr<UAISenseConfig_Damage> DamageConfig;

private:
	bool bSensingSight;
};

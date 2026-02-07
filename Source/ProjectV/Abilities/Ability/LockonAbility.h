// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Ability/ProjectVGameplayAbility.h"
#include "LockonAbility.generated.h"


UCLASS()
class PROJECTV_API ULockonAbility : public UProjectVGameplayAbility
{
	GENERATED_BODY()
	
public:
	ULockonAbility();

	// UGameplayAbility
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	// ~UGameplayAbility

/*
*	#####################################
*	#				Tick				#
* 	#####################################
*/
protected:
	UFUNCTION()
	void OnTick(float DeltaTime);

/*
*	#####################################
*	#				Input				#
* 	#####################################
*/
protected:
	void WaitInputPress();

	UFUNCTION()
	void OnInputPress(float TimeWaited);

/*
*	#####################################
*	#				Lockon				#
* 	#####################################
*/
protected:
	void WaitLockonChange();

	UFUNCTION()
	void OnReceiveLockonChangeLeft(FGameplayEventData Payload);

	UFUNCTION()
	void OnReceiveLockonChangeRight(FGameplayEventData Payload);

/*
*	#####################################
*	#				Functions			#
* 	#####################################
*/
protected:
	/* 첫 락온 대상을 결정하기 위한 트레이스를 수행하는 함수입니다. */
	void LockonTrace(UWorld* World);

	/* 락온 대상을 변경하기 위한 트레이스를 수행하는 함수입니다. */
	void TargetChangeLockonTrace(UWorld* World, float TraceAngle);
	
	/* 락온 대상을 입력받은 대상으로 설정합니다. */
	void SetLockonTarget(AActor* InActor);

	UFUNCTION(Server, Reliable)
	void SetLockonTarget_Server(AActor* InActor);
	void SetLockonTarget_Server_Implementation(AActor* InActor);

	/* 락온 대상을 초기화합니다. */
	void ClearTarget();

	UFUNCTION(Server, Reliable)
	void ClearTarget_Server();
	void ClearTarget_Server_Implementation();

private:
	/* 입력받은 HitResults 액터들중 화면 중앙과 가장 가까운 대상을 반환합니다. */
	AActor* FindBestActor(const TArray<FHitResult>& HitResults);

	/* 입력받은 HitResults 액터들 중 근처에 있는 대상을 찾습니다. */
	AActor* FindBestNearbyActor(const TArray<FHitResult>& HitResults, AActor* Pivot, TArray<AActor*> ActorToIgnore, bool bIsLeft = true);

	/* 입력받은 대상의 락온 위젯 가시성을 설정합니다. */
	void SetLockonWidgetVisibility(AActor* InActor, bool bIsVisible);

	/* 입력받은 대상이 락온 가능한 대상인지 판단합니다. */
	bool CanLockon(AActor* InActor) const;

protected:
	/* 락온을 위해 트레이스를 수행할 거리입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lockon Ability")
	float Range;

	/* 일반 트레이스에 실패시, 더 큰 범위로 트레이스하기 위해 사용되는 구체의 크기입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lockon Ability")
	float SphereRadius;

	/* 대상을 감지하기 위한 트레이스 채널입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lockon Ability")
	TEnumAsByte<ETraceTypeQuery> TraceChannel;

	/* 대상 사이에 벽이 있는지 체크하기 위한 트레이스 채널입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lockon Ability")
	TEnumAsByte<ETraceTypeQuery> WallCheckTraceChannel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lockon Ability|Debugging")
	bool bShowDebug;

private:
	TWeakObjectPtr<AActor> TargetActor;
};

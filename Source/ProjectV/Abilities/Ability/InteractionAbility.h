// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Ability/ProjectVGameplayAbility.h"
#include "InteractionAbility.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FInteractScannedEvent, ACharacter*, AActor*);

/**
 *	상호작용을 담당하는 어빌리티입니다.
 *	주변 대상을 탐색하고 활성화하는 동작입니다.
 */
UCLASS()
class PROJECTV_API UInteractionAbility : public UProjectVGameplayAbility
{
	GENERATED_BODY()
	
public:
	UInteractionAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION()
	void OnScan(AActor* PrevActor, AActor* NewActor);

public:
	static FInteractScannedEvent OnInteractScanned;

protected:
	/* 상호작용 가능한 대상을 스캔할 범위입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interact")
	float ScanRadius;

	/* 상호작용 가능한 대상이 있을때 캐릭터를 기준으로 허용할 각도입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interact")
	float ScanAngle;

	/* 스캔 간격입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interact")
	float ScanInterval;

	/* 디버깅 표시 여부입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interact|Debug")
	bool bShowScanDebug;

private:
	FTimerHandle DelayHandle;

	TWeakObjectPtr<AActor> CurrentScannedActor;
};

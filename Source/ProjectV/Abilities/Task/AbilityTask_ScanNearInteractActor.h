// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "UObject/ScriptInterface.h"
#include "AbilityTask_ScanNearInteractActor.generated.h"

class IInteractionInterface;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnScan, AActor*, PrevActor, AActor*, NewActor);

/**
 *	Task 실행자를 주변으로 인터렉션이 가능한 액터 하나를 찾음
 *  스캔된 액터는 캐싱되어 변경된 경우에만 호출됨
 * 
 *	 ScanRadius(float)	:	시전자를 기준으로 스캔할 범위입니다.
 *	 ScanInteral(float)	:	스캔을 반복할 시간입니다.
 *	 ScanAngle(float)	:	스캔을 허용할 각도입니다. 시전자의 정면 방향을 기준으로 측정합니다. (ex: 45 = 시전자 기준 좌 45도 우 45도 총 90도)
 */
UCLASS()
class PROJECTV_API UAbilityTask_ScanNearInteractActor : public UAbilityTask
{
	GENERATED_BODY()

public:
	UAbilityTask_ScanNearInteractActor();

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_ScanNearInteractActor* ScanNearInteractActor(UGameplayAbility* OwningAbility, float InScanRadius, float InScanAngle, float InScanInterval, bool bShowDebug);

public:
	UPROPERTY(BlueprintAssignable)
	FOnScan OnScan;

protected:
	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityIsEnding) override;

	void Scan();

private:
	// 오버랩된 결과를 필터링하고 제일 가까운 대상을 기준으로 정렬합니다.
	void FilterInteractable(AActor* InActor, TArray<FOverlapResult>& Result);

	// 상호작용 액터를 SourceObject로 등록하여 어빌리티를 부여합니다.
	void GiveInteractAbility(AActor* AvatarActor, AActor* SourceObject);

	// 현재 부여된 어빌리티가 부여된 상태라면, 회수합니다.
	void ClearInteractAbility(AActor* AvatarActor);

private:
	float ScanRadius;
	float ScanInterval;
	float ScanAngle;

	bool bShowDebug;

	bool bScanned;

	FTimerHandle Handle;

	TWeakObjectPtr<AActor> CachedInteractableActor;

	TMap<FObjectKey, FGameplayAbilitySpecHandle> GrantedAbilities;

	FGameplayAbilitySpecHandle GrantedAbilityHandle;
};

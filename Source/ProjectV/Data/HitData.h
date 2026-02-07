// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "HitData.generated.h"

class UGameplayAbility;

UENUM(BlueprintType)
enum class EPushType : uint8
{
	None				UMETA(DisplayName = "없음"),
	Attacker			UMETA(DisplayName = "공격자 기준"),
	Victim				UMETA(DisplayName = "피격자 기준"),
	World				UMETA(DisplayName = "월드 기준"),
	AttackerToVictim	UMETA(DisplayName = "공격자에서 피격자")
};

/**
 *	피해를 입히기위한 정보
 */
UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class PROJECTV_API UHitData : public UObject
{
	GENERATED_BODY()

public:
	UHitData();

	/** 
	* 일반적인 상황에서 넉백되어야 할 방향을 계산합니다.
	*
	* @param Attacker EPushType에서 영향을 미칠 공격자를 뜻합니다.
	* @param Victim EPushType에서 영향을 미칠 피격자를 뜻합니다.
	* @return 피격자를 이동시켜야 하는 Vector입니다.
	*/
	UFUNCTION(BlueprintPure, Category = "HitData")
	FVector GetPushDirection(AActor* Attacker, AActor* Victim) const;

	/**
	* 가드중 피격된 상황에서 넉백되어야 할 방향을 계산합니다.
	*
	* @param Attacker EPushType에서 영향을 미칠 공격자를 뜻합니다.
	* @param Victim EPushType에서 영향을 미칠 피격자를 뜻합니다.
	* @return 피격자를 이동시켜야 하는 Vector입니다.
	*/
	UFUNCTION(BlueprintPure, Category = "HitData")
	FVector GetGuardPushDirection(AActor* Attacker, AActor* Victim) const;

private:
	FVector CalculatePushDirection(AActor* Attacker, AActor* Victim, EPushType InPushType, FVector InPushAmount) const;

public:
	/* 공격 방향 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitData", meta = (Categories = "AttackDirection"))
	FGameplayTag AttackDirection;

	/* 피해타입 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitData", meta = (Categories = "Hit.MainType"))
	FGameplayTag HitMainType;

	/* 피해 서브효과 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitData", meta = (Categories = "Hit.SubType"))
	FGameplayTag HitSubType;

	/* 피격 성공시 발생할 이펙트 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitData")
	FName HitEffectName;

	/* 피격 성공시 발생할 사운드 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitData")
	FName HitSoundName;

	/* 데미지 배율 (1.0 = 공격력의 100% 피해)*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitData")
	float DamageRatio;

	/* 고정피해 (방어력 무시) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitData")
	float FixedDamage;

	/* 그로기 수치 증가량 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitData")
	float GroggyGauge;

	/* 대상을 미는 방향을 정하는 기준 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitData")
	EPushType PushType;

	/* 대상을 미는 힘 (X: 앞/뒤,   Y: 좌/우,    Z: 위/아래) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitData", meta = (EditCondition = "PushType != EPushType::None", EditConditionHides))
	FVector PushAmount;

	/* 가드상태의 대상을 미는 방향을 정하는 기준 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitData")
	EPushType GuardPushType;

	/* 가드상태의 대상을 미는 힘 (X: 앞/뒤,   Y: 좌/우,    Z: 위/아래) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitData", meta = (EditCondition = "GuardPushType != EPushType::None", EditConditionHides))
	FVector GuardPushAmount;

	/* 공격 성공시 공격자가 정지하는 시간 스케일 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitData|Effect")
	float AttackerHitStopScale;

	/* 공격 성공시 공격자가 정지하는 시간 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitData|Effect")
	float AttackerHitStopTime;

	/* 공격 성공시 피격자가 정지하는 시간 스케일 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitData|Effect")
	float VictimHitStopScale;

	/* 공격 성공시 피격자가 정지하는 시간 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitData|Effect")
	float VictimHitStopTime;

	/* 패링 허용여부 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitData")
	bool bCanParry;

	/* 적 패링활성 게이지 감소량 (음수값으로 감소, 모두 감소되면 적이 공격을 확정적으로 패링함) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitData", meta = (EditCondition = "bIsParry == true"))
	float ParryGauge;

	/* 패링당했을때 실행되는 어빌리티 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitData", meta = (EditCondition = "bIsParry == true"))
	TSubclassOf<UGameplayAbility> ParryAbility;
};

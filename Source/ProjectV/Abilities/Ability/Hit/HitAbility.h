// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Ability/ActionAbility.h"
#include "Kismet/KismetMathLibrary.h"
#include "HitAbility.generated.h"

class UHitData;
class UCombatCharacter;

UENUM(BlueprintType)
enum class EHitDirectionType : uint8
{
	Forward,
	Left,
	Right,
	Backward,
	End				UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FHitMontageRandomList
{
	GENERATED_BODY()

public:
	void Empty() { Montages.Empty(); }

	UAnimMontage* GetRandomMontage(float InServerWorldTimeSeconds) const
	{
		if (Montages.IsEmpty())
		{
			return nullptr;
		}

		if (Montages.Num() == 1)
		{
			return Montages[0];
		}

		// Server, Client가 동일한 랜덤값을 가지도록 동일한 시드 사용
		// 다양한 랜덤값을 추출하기 위해 시드에 10을 곱해서 초단위가 아닌 소수점 첫째자리까지 사용
		FRandomStream Stream((int32)(InServerWorldTimeSeconds * 10.f));
		int32 Idx = Stream.RandRange(0, Montages.Num() - 1);

		return Montages[Idx];
	}

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Random Montages")
	TArray<UAnimMontage*> Montages;
};

USTRUCT(BlueprintType)
struct FHitMontageSet
{
	GENERATED_BODY()

public:
	/* 공격방향에 따른 몽타주가 지정되지 않은 경우 기본적으로 적용될 몽타주입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> DefaultMontage;

	/* 공격방향에 따른 몽타주를 재생합니다. ( ※ DefaultMontage가 비어있는 경우 활성화됩니다. ) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ForceInlineRow, EditCondition = "DefaultMontage == nullptr", Categories = "AttackDirection"))
	TMap<FGameplayTag, FHitMontageRandomList> DirectionalMontages;
};


/**
 *	모든 피격 어빌리티의 기반이 되는 클래스입니다.
 *   - 피격 데이터 및 처리를 관리(공격자와의 각도, 넉백 등..)
 *   - 어빌리티 규칙 기본태그값 지정
 * 
 *	연결되는 피격(ex: Ground, AttackFail)을 제외하고 대부분의 Hit Ability는
 *	공격자에 대한 정보를 담기 위해 TriggerEventData로 트리거됩니다. (HitEvent.Result.{HitType})
 * 
 *	!! 서버에서 실행됩니다. !!
 */
UCLASS()
class PROJECTV_API UHitAbility : public UProjectVGameplayAbility
{
	GENERATED_BODY()
	
public:
	UHitAbility();

	// UActionAbility
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	// ~UActionAbility

protected:
	UFUNCTION(BlueprintPure, Category = "Hit Ability")
	ACombatCharacter* GetAttacker();

	UFUNCTION(BlueprintPure, Category = "Hit Ability")
	UHitData* GetHitData();

	UFUNCTION(BlueprintPure, Category = "Hit Ability")
	float GetHitAngle();

	UFUNCTION(BlueprintPure, Category = "Hit Ability")
	EHitDirectionType GetHitDirectionType();

	UFUNCTION(BlueprintCallable, Category = "Hit Ability")
	void ApplyPush(FVector KnockbackLocation, float Strength, float Duration);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Ability")
	TObjectPtr<UCurveFloat> KnockbackCurve;

	/* !! 아래 변수들을 직접 사용하여 조작하지 마세요. Get 함수에서 캐싱됩니다. !! */
private:
	TObjectPtr<ACombatCharacter> Attacker;
	TObjectPtr<UHitData> HitData;
	float HitAngle;
};

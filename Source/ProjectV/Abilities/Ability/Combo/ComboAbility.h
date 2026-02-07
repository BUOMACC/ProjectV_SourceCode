// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Ability/ProjectVGameplayAbility.h"
#include "Struct/ActorArray.h"
#include "ComboAbility.generated.h"

class UHitData;

/**
 *	모든 공격 어빌리티의 기반이 되는 클래스입니다.
 *   - 한 어빌리티에 대해 피격 처리를 관리
 *   - 어빌리티 규칙 기본태그값 지정
 * 
 *  기본설정이 지정된 아래 클래스들을 상속받으세요.
 *   - FirstComboAbility
 *	 - LinkComboAbility
 *	 - AirFirstComboAbility
 *	 - AirLinkComboAbility
 *	 - SkillComboAbility
 */
UCLASS()
class PROJECTV_API UComboAbility : public UProjectVGameplayAbility
{
	GENERATED_BODY()
	
/*
	######################################
	#			  Functions				 #
	######################################
*/
public:
	UComboAbility();

protected:
	// UGameplayAbility
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	// ~UGameplayAbility
	
	// UProjectVGameplayAbility
	virtual void OnEventReceived(FGameplayTag MatchingTag, const FGameplayEventData* Payload) override;
	// ~UProjectVGameplayAbility

	virtual void OnReceiveEvent_SuccessOverlapHitBox(FGameplayAbilityTargetDataHandle TargetData, UHitData* HitData);

protected:
	void ResetHitMap();

	UFUNCTION(BlueprintPure, Category = "Combo Abililty")
	bool CheckAlreadyHit(const UHitData* HitData, const FGameplayAbilityTargetDataHandle& TargetData);

	UFUNCTION(BlueprintCallable, Category = "Combo Ability")
	bool AddHitMap(const UHitData* HitData, const FGameplayAbilityTargetDataHandle& TargetData);

#pragma region Blueprint Functions
/*
	######################################
	#	    BP Response Functions		 #
	######################################
*/
protected:
	/* 히트박스에 오버랩된 대상을 받으면 서버에서 호출됩니다. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Combo Ability", DisplayName = "OnReceiveComboEvent_SuccessOverlapHitBox", meta = (ScriptName = "OnReceiveComboEvent_SuccessHit"))
	void K2_OnReceiveEvent_SuccessOverlapHitBox(FGameplayAbilityTargetDataHandle TargetData, UHitData* HitData);

#pragma endregion


/*
	######################################
	#			  Variables				 #
	######################################
*/
private:
	/*
	 * 해당 어빌리티에서 피격에 성공한 대상목록 (동일 어빌리티 내 중복피격 방지)
	 *  Key:	어떤 피격데이터로 피격을 했는지
	 *  Value:	피격데이터로 피격된 대상 목록
	*/
	UPROPERTY()
	TMap<UHitData*, FActorArray> HitMap;
};

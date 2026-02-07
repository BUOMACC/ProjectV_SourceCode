// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Ability/ProjectVGameplayAbility.h"
#include "ProjectVCommonEnums.h"
#include "DodgeSelectorAbility.generated.h"

#pragma region Memo
/*
*	# DodgeAbility 하나로 모든 방향 회피를 구현하지 않은 이유  ,
*	 -> DodgeAbility를 여러 방향으로 분리해두면,
*		AI에서 원하는 방향으로 회피기를 사용하도록 처리하기 직관적이기 때문임
*/
#pragma endregion

/**
 *	회피기를 선택하는 로직만 포함된 어빌리티입니다.
 *	플레이어를 기준으로 아래 방식대로 회피기가 동작합니다.
 *	
 *	DodgeSelectorAbility -> 방향에 따라 DodgeAbility 선택 -> 선택된 DodgeAbility 실행
 * 
 *	( * 서버가 아닌 로컬 클라이언트를 기준으로 선택됨 * )
 * 
 */
UCLASS()
class PROJECTV_API UDodgeSelectorAbility : public UProjectVGameplayAbility
{
	GENERATED_BODY()
	
public:
	UDodgeSelectorAbility();

protected:
	// UGameplayAbility
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	// ~UGameplayAbility

	TSubclassOf<UGameplayAbility> SelectDodgeAbility();
	EDirection ConvertAccelerationToDirection(const FVector& InAcceleration);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Dodge Selector Ability", meta = (ArraySizeEnum = "EDirection"))
	TSubclassOf<UGameplayAbility> DirectionalDodgeAbility[(int32)EDirection::End];
};

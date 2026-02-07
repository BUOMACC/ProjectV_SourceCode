// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Character/PlayerComponent.h"
#include "Character/PlayerCharacter.h"
#include "CombatGameplayTags.h"
#include "Data/HitData.h"
#include "ProjectVGameplayAbility.generated.h"

/*
*	Ability 사용시 소유자 및 전투에 관련된 기본 공통로직이 포함됨
*	가장 최상위 Ability
*/
UCLASS()
class PROJECTV_API UProjectVGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
/*
	######################################
	#			  Functions				 #
	######################################
*/
public:
	UProjectVGameplayAbility();

	/* CurrentActorInfo의 AvatarActor로부터 ACombatCharacter를 얻어옵니다. */
	UFUNCTION(BlueprintPure, Category = "ProjectV GameplayAbility")
	ACombatCharacter* GetCombatCharacter() const;

	/* CurrentActorInfo의 AvatarActor로부터 UCharacterMovementComponent를 얻어옵니다. */
	UFUNCTION(BlueprintPure, Category = "ProjectV GameplayAbility")
	UCharacterMovementComponent* GetCharacterMovementComponent() const;

	/* CurrentActorInfo의 AvatarActor로부터 ACombatPlayerController를 얻어옵니다. */
	UFUNCTION(BlueprintPure, Category = "ProjectV GameplayAbility")
	ACombatPlayerController* GetCombatPlayerController() const;

	/* CurrentActorInfo의 AvatarActor로부터 UPlayerComponent를 얻어옵니다. */
	UFUNCTION(BlueprintPure, Category = "ProjectV GameplayAbility")
	UPlayerComponent* GetPlayerComponent() const;

	/* APlayerComponent로부터 소유 클라이언트에 저장된 InputBuffer를 가져옵니다. */
	UFUNCTION(BlueprintPure, Category = "ProjectV GameplayAbility")
	bool GetLocalInputBuffer(FGameplayTag& OutInputBuffer) const;

	/* CurrentActorInfo로부터 AController를 얻어옵니다. */
	UFUNCTION(BlueprintPure, Category = "ProjectV GameplayAbility")
	AController* GetActorController() const;

protected:
	// UGameplayAbility
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	// ~UGameplayAbility

	void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;

	/* 이벤트를 받으면 소유 클라이언트에서 호출됩니다. */
	UFUNCTION(BlueprintImplementableEvent, Category = "ProjectV GameplayAbility", DisplayName = "OnReceiveComboEvent", meta = (ScriptName = "OnReceiveComboEvent"))
	void K2_OnEventReceived(FGameplayTag MatchingTag, FGameplayEventData Payload);
	virtual void OnEventReceived(FGameplayTag MatchingTag, const FGameplayEventData* Payload);


	/*
		######################################
		#			  Variables				 #
		######################################
	*/
public:
	/* 입력을 받을 태그 (첫 입력을 통한 공격이나 입력을 유지하고 있는지 체크할때 사용함) */
	UPROPERTY(EditDefaultsOnly, Category = "ProjectV GameplayAbility", meta = (Categories = "Input"))
	FGameplayTag InputTag;

	/* InputTag에 해당하는 입력을 받은 경우 자동으로 실행할지? */
	UPROPERTY(EditDefaultsOnly, Category = "ProjectV GameplayAbility")
	bool bAutoTriggerWhenInput;

	/* 어빌리티가 부여되면 자동으로 실행합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "ProjectV GameplayAbility")
	bool bAutoActivated;

protected:
	FDelegateHandle ReceiveEventCallbackHandle;
};

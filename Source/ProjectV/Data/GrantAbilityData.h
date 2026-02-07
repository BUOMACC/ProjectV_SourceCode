// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "GameplayAbilitySpec.h"
#include "GrantAbilityData.generated.h"

class UGameplayAbility;
class UGameplayEffect;

UENUM(BlueprintType)
enum class EGrantedAbilityRule : uint8
{
	/* 어빌리티를 즉시 회수합니다. */
	Instant,
	/* 어빌리티가 실행 중이라면, 끝날 때까지 대기 후 회수합니다. */
	Pending,
};

USTRUCT(BlueprintType)
struct FGrantedAbilityHandles
{
	GENERATED_BODY()

public:
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);

	void TakeFromAbilitySystem(UAbilitySystemComponent* CombatASC);

	bool CanRemoveNow(UAbilitySystemComponent* CombatASC) const;

protected:
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;

public:
	EGrantedAbilityRule AbilityTakeRule;
};

/**
 *	부여할 어빌리티 목록 (시작시 기본 데이터를 부여할때 사용)
 */
UCLASS()
class PROJECTV_API UGrantAbilityData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UGrantAbilityData();

public:
	/* 이 태그를 가지면, 부여시 동일한 태그를 가진 데이터와 스왑합니다. */
	UPROPERTY(EditAnywhere, Category = "Grant Abilities", meta = (Categories = "GrantAbility"))
	FGameplayTag TypeTag;

	/* 어빌리티 회수 규칙입니다. */
	UPROPERTY(EditAnywhere, Category = "Grant Abilities")
	EGrantedAbilityRule AbilityTakeRule;

	/* 부여할 GameplayAbility 목록입니다. */
	UPROPERTY(EditAnywhere, Category = "Grant Abilities")
	TArray<TSubclassOf<UGameplayAbility>> GrantAbilities;

	/* 부여할 GameplayEffect 목록입니다. */
	UPROPERTY(EditAnywhere, Category = "Grant Abilities")
	TArray<TSubclassOf<UGameplayEffect>> GrantGameplayEffects;
};

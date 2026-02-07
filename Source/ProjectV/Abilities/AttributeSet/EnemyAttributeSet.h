// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemComponent.h"
#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "EnemyAttributeSet.generated.h"

#pragma region Memo
/*
*	# Call Function Pipeline
*	- PreGameplayEffectExecute
*	- PreAttributeBaseChange
*	- PreAttributeChange
*	- PostAttributeChange
*	- PostAttributeBaseChange
*	- PostGameplayEffectExecute
*/
#pragma endregion



// Helper Macros
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


/*
 *	Enemy Attribute 관련
 *	- 적에게 필요한 데이터 (패링 게이지, 시야각...)를 포함하는 AttributeSet
 */
UCLASS()
class PROJECTV_API UEnemyAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
/*
	######################################
	#			  Functions				 #
	######################################
*/
public:
	UEnemyAttributeSet();

	/* Generate Getter / Setter */
	ATTRIBUTE_ACCESSORS(UEnemyAttributeSet, ParryGauge);
	ATTRIBUTE_ACCESSORS(UEnemyAttributeSet, Life);
	ATTRIBUTE_ACCESSORS(UEnemyAttributeSet, MaxLife);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	/* Attribute가 변경되기 전에 호출 (CurrentValue) */
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	
	/* Attribute가 변경된 이후 호출 (CurrentValue) */
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	/* GameplayEffect가 적용되기 이전에 호출 -> 특정 GameplayEffect를 무시해야하는 경우 로직을 이곳에서 처리 */
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;

	/* GameplayEffect가 적용된 이후에 호출 -> BaseValue가 변경된 이후 (Instant Only) */
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	/* BaseValue가 변경되기 전에 호출 (Instant Only) */
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	/* BaseValue가 변경된 이후 호출 */
	virtual void PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const override;

protected:
	/* 어트리뷰트 값 제한 */
	void ClampValue(const FGameplayAttribute& Attribute, float& Value) const;

private:
	/* RepNotify */
	UFUNCTION()
	void OnRep_Life(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxLife(const FGameplayAttributeData& OldValue);

/*
	######################################
	#			  Variables				 #
	######################################
*/
private:
	UPROPERTY(BlueprintReadOnly, Category = "Enemy Stat", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ParryGauge;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Life, Category = "Enemy Stat", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Life;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxLife, Category = "Enemy Stat", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxLife;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemComponent.h"
#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "CombatAttributeSet.generated.h"

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

/**
 *	Character Stat
 */
UCLASS()
class PROJECTV_API UCombatAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
/*
	######################################
	#			  Functions				 #
	######################################
*/
public:
	UCombatAttributeSet();

	/* Generate Getter / Setter */
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, Stamina);
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, MaxStamina);
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, GuardAngle);
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, GroggyGauge);
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, MaxGroggyGauge);

public:
	/* Events */
	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnGroggy, AActor* /* Instigator */, AActor* /* Causer */, const FGameplayEffectSpec&)
	FOnGroggy OnGroggy;

protected:
	/* Override */
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	/* GameplayEffect가 적용되기 이전에 호출 (Instant Only) */
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;

	/* GameplayEffect가 적용된 이후에 호출 -> BaseValue가 변경된 이후 (Instant Only) */
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	/* Attribute가 변경되기 전에 호출 (CurrentValue) */
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	/* Attribute가 변경된 이후 호출 (CurrentValue) */
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

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
	void OnRep_Stamina(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_GuardAngle(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_GroggyGauge(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxGroggyGauge(const FGameplayAttributeData& OldValue);

/*
	######################################
	#			  Variables				 #
	######################################
*/
private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, Category = "Combat Stat", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Stamina;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStamina, Category = "Combat Stat", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxStamina;

	/* 좌/우 합한 값이 가드가능 각도 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_GuardAngle, Category = "Combat Stat", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData GuardAngle;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_GroggyGauge, Category = "Combat Stat", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData GroggyGauge;

	/* GroggyGauge가 이값에 도달하면 그로기상태가 됨 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxGroggyGauge, Category = "Combat Stat", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxGroggyGauge;
};

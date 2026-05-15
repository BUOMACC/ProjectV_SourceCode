// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemComponent.h"
#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "HealthAttributeSet.generated.h"

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
 *	Health 관련
 *	- 추후 파괴가능한 오브젝트 등은 Stamina 등, 다른 스텟이 필요하지 않으므로
 *	  별도의 HealthAttribute로 분할함
 */
UCLASS()
class PROJECTV_API UHealthAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
/*
	######################################
	#			  Functions				 #
	######################################
*/
public:
	UHealthAttributeSet();

	/* Generate Getter / Setter */
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, ReceivedDamage);
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, WeaponDamage);
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, FixedDamage);

public:
	/* Events */
	DECLARE_MULTICAST_DELEGATE_FourParams(FOnDamaged, AActor* /* Instigator */, AActor* /* SourceObjectActor */, const FGameplayEffectSpec&, float)
	FOnDamaged OnDamaged;

	DECLARE_MULTICAST_DELEGATE_FourParams(FOnOutOfHealth, AActor* /* Instigator */, AActor* /* SourceObjectActor */, const FGameplayEffectSpec&, float)
	FOnOutOfHealth OnOutOfHealth;

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
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

/*
	######################################
	#			  Variables				 #
	######################################
*/
private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Combat Stat", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Combat Stat", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	/* 예측이 필요없는 값은 Replicated로 생성할 필요가 없다. (Health -= ReceivedDamage) */
	UPROPERTY(BlueprintReadOnly, Category = "Combat Stat", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ReceivedDamage;

private:
	UPROPERTY(BlueprintReadOnly, Category = "Combat Stat", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData WeaponDamage;

	UPROPERTY(BlueprintReadOnly, Category = "Combat Stat", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData FixedDamage;
};

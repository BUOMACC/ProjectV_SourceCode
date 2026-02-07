// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "GameplayTagContainer.h"
#include "Data/GrantAbilityData.h"
#include "CombatAbilitySystemComponent.generated.h"

class UGameplayAbility;

/**
 *	ProjectV Ability System Component
 *	 - 입력에 대한 제어, 어빌리티 데이터세트 부여등의 추가역할 수행
 */
UCLASS(ClassGroup = AbilitySystem, hidecategories = (Object, LOD, Lighting, Transform, Sockets, TextureStreaming), editinlinenew, meta = (BlueprintSpawnableComponent))
class PROJECTV_API UCombatAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
/*
	######################################
	#			  Functions				 #
	######################################
*/
public:
	UCombatAbilitySystemComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AbilityInputPressedTag(const FGameplayTag& InputTag);
	void AbilityInputReleasedTag(const FGameplayTag& InputTag);
	void ProcessAbilityInput();

	/* GameplayAbility와 GameplayEffect를 묶은 데이터를 일괄 부여합니다. */
	bool ApplyGrantAbilityData(const UGrantAbilityData* GrantAbilityData, UObject* SourceObject = nullptr);
	
	/*
	*	입력받은 Tag에 일치하는 어빌리티 묶음 데이터가 적용되어있으면, 회수합니다.
	*	 @param GrantAbilityTypeTag : GrantAbility.{TypTag} (GrantAbility로 시작하는 Tag입니다.)
	*/
	bool TakeGrantAbilityData(const FGameplayTag& GrantAbilityTypeTag);

	bool IsGrantedAbilityData(const FGameplayTag& GrantAbilityTypeTag) const;

protected:
	// UAbilitySystemComponent Interface
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;
	virtual void ApplyAbilityBlockAndCancelTags(const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility, bool bEnableBlockTags, const FGameplayTagContainer& BlockTags, bool bExecuteCancelTags, const FGameplayTagContainer& CancelTags) override;
	// ~ UAbilitySystemComponent Interface

/*
	######################################
	#			  Variables				 #
	######################################
*/
protected:
	TArray<FGameplayAbilitySpecHandle> AbilityInputPressedHandles;
	TArray<FGameplayAbilitySpecHandle> AbilityInputReleasedHandles;
	TArray<FGameplayAbilitySpecHandle> AbilityInputHeldHandles;

private:
	/*
	*	Key: GrantedAbility.{TypeTag} (FGameplayTag)
	*	
	*	 용도: 동일한 Key값을 가진 Ability DataSet가 부여되었을 경우를 제어하기 위함
	*	 동작: 기존에 있는 데이터는 회수되고 새로 부여됨 (Swap)
	*/
	TMap<FGameplayTag, FGrantedAbilityHandles> GrantAbilityDataMap;

	TMap<FGameplayTag, FGrantedAbilityHandles> GrantAbilityDataPendingMap;
};

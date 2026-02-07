// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectVGameplayAbility.h"
#include "ActionAbility.generated.h"


class UAnimMontage;

/*
*	Animation 기능이 추가된 어빌리티
*	상호작용, 공격, 피격어빌리티는 이 클래스를 파생하여 구현
* 
*	 ※ Client - Side 예측으로 애니메이션을 재생하기 때문에,
*		RPC를 통해 Animation을 하나의 어빌리티 내에서 선택하게 하는경우 문제가 발생할 가능성이 높음
* 
* 
*	TODO: **** DEPRECATED ****
*/
UCLASS()
class PROJECTV_API UActionAbility : public UProjectVGameplayAbility
{
	GENERATED_BODY()
	
/*
	######################################
	#			  Functions				 #
	######################################
*/
public:
	UActionAbility();

protected:
	/* 각각 Animation 실행전, 후(즉시)에 호출됩니다. */
	virtual void PrePlayAnimation();
	virtual void PostPlayAnimation();

	/*
	 * !! override한 뒤, 실행할 Animation을 반환하세요.
	 *  별도의 함수로 뺀 이유는, 몽타주 선택에 복잡한 조건이 포함될 수 있기 때문입니다.
	*/
	virtual UAnimMontage* GetAnimMontage() const;

	UFUNCTION()
	void OnMontageBlendingOut();

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageEnded();

	UFUNCTION()
	void OnMontageCanceled();

#pragma region Blueprint Functions
/*
	######################################
	#	    BP Response Functions		 #
	######################################
*/
protected:
	/* Animation 실행전에 호출됩니다. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Action Ability", meta = (DisplayName = "PrePlayAnimation"))
	void K2_PrePlayAnimation();

	/* Animation 실행후 즉시 호출됩니다. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Action Ability", meta = (DisplayName = "PostPlayAnimation"))
	void K2_PostPlayAnimation();

	UFUNCTION(BlueprintImplementableEvent, Category = "Action Ability", meta = (DisplayName = "OnMontageBlendingOut"))
	void K2_OnMontageBlendingOut();

	UFUNCTION(BlueprintImplementableEvent, Category = "Action Ability", meta = (DisplayName = "OnMontageInterrupted"))
	void K2_OnMontageInterrupted();

	UFUNCTION(BlueprintImplementableEvent, Category = "Action Ability", meta = (DisplayName = "OnMontageEnded"))
	void K2_OnMontageEnded();

	UFUNCTION(BlueprintImplementableEvent, Category = "Action Ability", meta = (DisplayName = "OnMontageCanceled"))
	void K2_OnMontageCanceled();

	UFUNCTION(BlueprintImplementableEvent, Category = "Action Ability", meta = (DisplayName = "GetActionMontage"))
	UAnimMontage* K2_GetActionMontage() const;

#pragma endregion



/*
	######################################
	#			  Variables				 #
	######################################
*/
protected:
	/*
	*	Animation이 끝나면 어빌리티를 End처리 할까요?
	*	명시적으로 End Ability를 호출해야 한다면, 이 옵션을 비활성화 하세요.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Action Ability|Animation Settings")
	bool bEndAbilityWhenAnimationEnd;

	/* Ability가 종료되면 Animation을 중단할까요? */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Action Ability|Animation Settings")
	bool bStopWhenAbilityEnd;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Action Ability|Animation Settings")
	float PlayRate;

private:
	FDelegateHandle InterruptedHandle;
	UAnimMontage* MontageToPlay;
};

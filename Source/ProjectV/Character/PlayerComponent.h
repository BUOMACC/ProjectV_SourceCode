// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputActionValue.h"
#include "GameplayTagContainer.h"
#include "PlayerComponent.generated.h"

class UInputAction;

/*
*	캐릭터의 입력을 자동으로 바인딩해 조작할수 있게 해줌
*	캐릭터에 부착하면 이동및 마우스 회전같은 컨트롤이 가능해짐
* 
*	캐릭터마다 다른 기능을 동작하게 하고 싶다면 이 컴포넌트를 확장해 바인딩 로직을 변경해야 함
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTV_API UPlayerComponent : public UActorComponent
{
	GENERATED_BODY()

/*
	######################################
	#			  Functions				 #
	######################################
*/
public:	
	UPlayerComponent();
	virtual void BindInputs(class UInputComponent* PlayerInputComponent);

	UFUNCTION(BlueprintCallable, Category = "PlayerComponent|Input")
	void EnableInputBuffer();

	UFUNCTION(BlueprintCallable, Category = "PlayerComponent|Input")
	void DisableInputBuffer();

	UFUNCTION(BlueprintCallable, Category = "PlayerComponent|Input")
	void ClearInputBuffer();

	UFUNCTION(BlueprintPure, Category = "PlayerComponent|Input")
	const FGameplayTag& GetInputBuffer();

protected:
	virtual void BeginPlay() override;

	virtual void Input_Move(const FInputActionValue& InputValue);
	virtual void Input_Look(const FInputActionValue& InputValue);

	virtual void Input_QuickSlot_ConsumptionPrev();
	virtual void Input_QuickSlot_ConsumptionNext();

	virtual void Input_QuickSlot_SkillPrev();
	virtual void Input_QuickSlot_SkillNext();

	virtual void Input_LockonChange(const FInputActionValue& InputValue);

	virtual void Input_AbilityPressed(FGameplayTag AbilityInputTag);
	virtual void Input_AbilityReleased(FGameplayTag AbilityInputTag);
	virtual void Input_AbilityStarted(FGameplayTag AbilityInputTag);

/*
	######################################
	#			  Variables				 #
	######################################
*/
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputTagMappingConfig> InputConfig;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

private:
	UPROPERTY()
	TObjectPtr<class ACharacter> OwnerCharacter;

	UPROPERTY()
	TObjectPtr<class AController> OwnerController;

	FGameplayTag InputBuffer;
	bool bEnableInputBuffer;
};

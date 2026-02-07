// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "Input/InputTagMappingConfig.h"
#include "CombatEnhancedInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTV_API UCombatEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

/*
	######################################
	#			  Functions				 #
	######################################
*/
public:
	template<typename UserClass, typename Pressed, typename Released, typename Started>
	void BindAutoAction(const UInputTagMappingConfig* InputConfig, UserClass* Object, Pressed PressedEvent, Released ReleasedEvent, Started StartedEvent)
	{
		check(InputConfig);
		for (const FInputActionMappingData& Action : InputConfig->AutoBindInputActions)
		{
			if (Action.InputAction && Action.InputTag.IsValid())
			{
				if (PressedEvent)
				{
					BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedEvent, Action.InputTag);
				}

				if (ReleasedEvent)
				{
					BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedEvent, Action.InputTag);
				}

				if (StartedEvent)
				{
					BindAction(Action.InputAction, ETriggerEvent::Started, Object, StartedEvent, Action.InputTag);
				}
			}
		}
	}

	template<typename UserClass, typename Func>
	void BindManualAction(const UInputTagMappingConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, Func InputEvent)
	{
		check(InputConfig);
		if (const UInputAction* Action = InputConfig->FindManualInputActionByTag(InputTag))
		{
			BindAction(Action, TriggerEvent, Object, InputEvent);
		}
	}
};

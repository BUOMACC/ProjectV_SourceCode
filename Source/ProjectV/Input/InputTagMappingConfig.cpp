// Fill out your copyright notice in the Description page of Project Settings.


#include "InputTagMappingConfig.h"
#include "InputAction.h"

const UInputAction* UInputTagMappingConfig::FindManualInputActionByTag(const FGameplayTag& Tag) const
{
	for (const FInputActionMappingData& Action : ManualBindInputActions)
	{
		if (Action.InputAction && Action.InputTag == Tag)
		{
			return Action.InputAction;
		}
	}

#if !UE_BUILD_SHIPPING
	UE_LOG(LogTemp, Warning, TEXT("Not Found InputAction! (UInputTagMappingConfig)"));
#endif
	return nullptr;
}
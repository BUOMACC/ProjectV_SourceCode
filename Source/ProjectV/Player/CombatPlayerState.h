// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "CombatPlayerState.generated.h"

class UAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class PROJECTV_API ACombatPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

/*
	######################################
	#			  Functions				 #
	######################################
*/
public:
	// Ability System Interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// ~Ability System Interface
};

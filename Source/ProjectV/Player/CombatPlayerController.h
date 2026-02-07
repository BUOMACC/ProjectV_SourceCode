// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Abilities/CombatAbilitySystemComponent.h"
#include "Character/CombatCharacter.h"
#include "CombatPlayerController.generated.h"

class UProjectVUserSettingsShared;

UCLASS()
class PROJECTV_API ACombatPlayerController : public APlayerController
{
	GENERATED_BODY()

/*
	######################################
	#			  Functions				 #
	######################################
*/
public:
	virtual void BeginPlay() override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void AcknowledgePossession(class APawn* P) override;
	virtual void SetPlayer(UPlayer* InPlayer) override;

	UProjectVUserSettingsShared* GetSharedSettings();

public:
	UFUNCTION(BlueprintPure, Category = "Combat PlayerController")
	UCombatAbilitySystemComponent* GetCombatAbilitySystemComponent() const;

	UFUNCTION(BlueprintPure, Category = "Combat PlayerController")
	ACombatCharacter* GetControlledCharacter() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "Combat PlayerController")
	void OnAfterPossessClient();

/*
	######################################
	#			  Variables				 #
	######################################
*/
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActor> LockonTarget;

private:
	UPROPERTY()
	TObjectPtr<UProjectVUserSettingsShared> SharedSettings;
};

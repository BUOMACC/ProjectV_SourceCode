// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ProjectVGameState.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStateEvent, APlayerState*);

UCLASS()
class PROJECTV_API AProjectVGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AProjectVGameState();

	virtual void AddPlayerState(APlayerState* PlayerState) override;
	virtual void RemovePlayerState(APlayerState* PlayerState) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION(BlueprintCallable)
	void SetTeamKill(bool bFlag);

	UFUNCTION(BlueprintPure)
	bool CanTeamKill() const;

private:
	UPROPERTY(EditDefaultsOnly, Replicated, Category = "Rule")
	bool bAllowTeamKill;

public:
	FOnPlayerStateEvent OnPlayerStateAdded;
	FOnPlayerStateEvent OnPlayerStateRemoved;

private:
	FTimerHandle TimerHandle;
};

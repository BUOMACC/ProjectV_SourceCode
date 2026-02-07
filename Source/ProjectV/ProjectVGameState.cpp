// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectVGameState.h"
#include "ProjectVLogs.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

AProjectVGameState::AProjectVGameState()
{
	bAllowTeamKill = false;
	TimerHandle.Invalidate();
}

void AProjectVGameState::AddPlayerState(APlayerState* PlayerState)
{
	// Determine whether it should go in the active or inactive list
	if (!PlayerState->IsInactive())
	{
		// make sure no duplicates
		PlayerArray.AddUnique(PlayerState);
		
		// notify new player state added.
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [=]() {
			OnPlayerStateAdded.Broadcast(PlayerState);
		}, 1.f, false);
	}
}

void AProjectVGameState::RemovePlayerState(APlayerState* PlayerState)
{
	for (int32 i = 0; i < PlayerArray.Num(); i++)
	{
		if (PlayerArray[i] == PlayerState)
		{
			// notify new player state removed.
			OnPlayerStateRemoved.Broadcast(PlayerArray[i]);

			PlayerArray.RemoveAt(i, 1);
			return;
		}
	}
}

void AProjectVGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AProjectVGameState, bAllowTeamKill);
}

void AProjectVGameState::SetTeamKill(bool bFlag)
{
	if (!HasAuthority())
	{
		UE_LOG(LogProjectV, Warning, TEXT("AProjectVGameState::SetTeamKill - 팀 공격 옵션은 서버에서만 설정할 수 있습니다."));
		return;
	}

	if (bAllowTeamKill != bFlag)
	{
		bAllowTeamKill = bFlag;
	}
}

bool AProjectVGameState::CanTeamKill() const
{
	return bAllowTeamKill;
}
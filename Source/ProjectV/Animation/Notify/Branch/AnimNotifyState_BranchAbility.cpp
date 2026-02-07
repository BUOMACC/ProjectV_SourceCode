// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_BranchAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character/PlayerCharacter.h"
#include "Character/PlayerComponent.h"

UAnimNotifyState_BranchAbility::UAnimNotifyState_BranchAbility()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(240, 255, 240);
#endif

	ConditionOperator = EConditionOperator::OR;
}

FString UAnimNotifyState_BranchAbility::GetNotifyName_Implementation() const
{
	return TEXT("Branch");
}

void UAnimNotifyState_BranchAbility::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!CheckRule(MeshComp->GetOwner())) return;

	ProcessBranchAbility(MeshComp->GetOwner());
}

void UAnimNotifyState_BranchAbility::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (!CheckRule(MeshComp->GetOwner())) return;
	ProcessBranchAbility(MeshComp->GetOwner());
}

void UAnimNotifyState_BranchAbility::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!CheckRule(MeshComp->GetOwner())) return;
}

void UAnimNotifyState_BranchAbility::ProcessBranchAbility(AActor* Actor)
{
	// Only Player
	if (ACombatCharacter* CombatCharacter = Cast<ACombatCharacter>(Actor))
	{
		// 자신이 컨트롤중이거나, 서버인 경우에만 체크
		if (CombatCharacter->IsLocallyControlled() || CombatCharacter->HasAuthority())
		{
			// Condition Check
			bool bSuccess = false;
			for (UBranchCondition_Base* Condition : Conditions)
			{
				if (Condition == nullptr)
					continue;

				bool bConditionCheckResult = Condition->CheckCondition(CombatCharacter);
				if (Condition->bReverseResult)
				{
					bConditionCheckResult = !bConditionCheckResult;
				}

				// 1) 모든 조건을 만족해야 하는데, 하나라도 실패한 경우 실패 처리
				if (ConditionOperator == EConditionOperator::AND && !bConditionCheckResult)
				{
					bSuccess = false;
					break;
				}

				// 2) 하나라도 조건을 만족하면 되는 경우
				if (ConditionOperator == EConditionOperator::OR && bConditionCheckResult)
				{
					bSuccess = true;
					break;
				}
			}

			// 성공시, 액션 실행
			if (bSuccess)
			{
				// Clear InputBuffer if Player
				if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(CombatCharacter))
				{
					if (UPlayerComponent* PlayerComp = PlayerCharacter->GetPlayerComponent())
					{
						PlayerComp->ClearInputBuffer();
						PlayerComp->DisableInputBuffer();
					}
				}

				// Execute Action
				if (BranchAction)
				{
					BranchAction->Activate(CombatCharacter);
				}
			}
		}
	}
}

bool UAnimNotifyState_BranchAbility::CheckRule(AActor* Actor) const
{
	// If stand alone
	if (IsNetworked(Actor) == false)
		return true;

	switch (Rule)
	{
	case EBranchRule::OwningClient:
		return IsClient(Actor);

	case EBranchRule::Server:
		return IsServer(Actor);
	}

	return false;
}

bool UAnimNotifyState_BranchAbility::IsNetworked(AActor* Actor) const
{
	if (Actor == nullptr)
		return false;

	return Actor->GetNetMode() != ENetMode::NM_Standalone && Actor->GetLocalRole() != ENetRole::ROLE_SimulatedProxy;
}

bool UAnimNotifyState_BranchAbility::IsClient(AActor* Actor) const
{
	if (Actor == nullptr)
		return false;

	bool bIsLocallyControlled = false;
	if (const APawn* PawnOwner = Cast<APawn>(Actor))
	{
		bIsLocallyControlled = PawnOwner->IsLocallyControlled();
	}

	return bIsLocallyControlled;
	//return !IsServer(Actor) || bIsLocallyControlled || Actor->GetWorld()->GetNetMode() == ENetMode::NM_ListenServer;
}

bool UAnimNotifyState_BranchAbility::IsServer(AActor* Actor) const
{
	if (Actor == nullptr)
		return false;

	return (Actor->GetLocalRole() == ENetRole::ROLE_Authority);
	//return !IsNetworked(Actor) ||
	//		(IsNetworked(Actor) && Actor->GetLocalRole() == ENetRole::ROLE_Authority);
}
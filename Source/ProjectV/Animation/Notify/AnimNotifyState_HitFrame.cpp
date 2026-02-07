// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_HitFrame.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character/CombatCharacter.h"
#include "CombatGameplayTags.h"
#include "ProjectVGameState.h"

UAnimNotifyState_HitFrame::UAnimNotifyState_HitFrame()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(255, 120, 0);
#endif
}

FString UAnimNotifyState_HitFrame::GetNotifyName_Implementation() const
{
	return TEXT("HitFrame");
}

void UAnimNotifyState_HitFrame::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	ACombatCharacter* CombatCharacter = Cast<ACombatCharacter>(MeshComp->GetOwner());
	if (CombatCharacter == nullptr)
	{
		return;
	}

	UCombatAbilitySystemComponent* ASC = CombatCharacter->GetCombatAbilitySystemComponent();
	if (ASC == nullptr)
	{
		return;
	}

	ShapeCheck(CombatCharacter, ASC);
}

void UAnimNotifyState_HitFrame::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (bUseTick)
	{
		ACombatCharacter* CombatCharacter = Cast<ACombatCharacter>(MeshComp->GetOwner());
		if (CombatCharacter == nullptr)
		{
			return;
		}

		UCombatAbilitySystemComponent* ASC = CombatCharacter->GetCombatAbilitySystemComponent();
		if (ASC == nullptr)
		{
			return;
		}

		ShapeCheck(CombatCharacter, ASC);
	}
}

void UAnimNotifyState_HitFrame::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}

void UAnimNotifyState_HitFrame::ShapeCheck(ACombatCharacter* CombatCharacter, UCombatAbilitySystemComponent* CombatASC)
{
	if (Shape == nullptr || CombatCharacter == nullptr || CombatASC == nullptr)
	{
		return;
	}

	// Only Server
	if (CombatCharacter->HasAuthority())
	{
		TArray<FHitResult> Results;
		Shape->CheckOverlap(CombatCharacter, Results);
		if (Results.Num() > 0)
		{
			// Send Success Hit Event To Ability
			for (const FHitResult& HitResult : Results)
			{
				if (!CanTeamAttack(CombatCharacter, HitResult.GetActor()))
					continue;

				FGameplayAbilityTargetDataHandle TargetDataHandle = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(HitResult);
				FGameplayEventData Payload;
				Payload.TargetData = TargetDataHandle;
				Payload.Instigator = CombatCharacter;
				Payload.Target = HitResult.GetActor();
				Payload.ContextHandle = CombatASC->MakeEffectContext();
				Payload.OptionalObject = HitData;

				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(CombatCharacter, FCombatGameplayTags::Get().Event_Combat_SuccessOverlapHitBox, Payload);
			}
		}
	}
}

bool UAnimNotifyState_HitFrame::CanTeamAttack(AActor* InAttacker, AActor* InVictim) const
{
	if (!IsValid(InAttacker) || !IsValid(InVictim))
		return false;

	const AProjectVGameState* GameState = InAttacker->GetWorld()->GetGameState<AProjectVGameState>();
	ETeamAttitude::Type Attitude = FGenericTeamId::GetAttitude(InAttacker, InVictim);
	if (Attitude == ETeamAttitude::Friendly && !GameState->CanTeamKill())
	{
		return false;
	}

	return true;
}

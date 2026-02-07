// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_HitOneFrame.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character/CombatCharacter.h"
#include "CombatGameplayTags.h"
#include "ProjectVGameState.h"

UAnimNotify_HitOneFrame::UAnimNotify_HitOneFrame()
{
#if WITH_EDITORONLY_DATA
	// TODO: Set Notify Color Here
#endif
}

void UAnimNotify_HitOneFrame::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (Shape == nullptr)
	{
		return;
	}

	ACombatCharacter* CombatCharacter = Cast<ACombatCharacter>(MeshComp->GetOwner());
	if (CombatCharacter == nullptr)
	{
		return;
	}

	UAbilitySystemComponent* ASC = CombatCharacter->GetAbilitySystemComponent();
	if (ASC == nullptr)
	{
		return;
	}

	// Only Server
	if (CombatCharacter->HasAuthority())
	{
		TArray<FHitResult> Results;
		Shape->CheckOverlap(MeshComp->GetOwner(), Results);
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
				Payload.ContextHandle = ASC->MakeEffectContext();

				Payload.OptionalObject = HitData;
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(CombatCharacter, FCombatGameplayTags::Get().Event_Combat_SuccessOverlapHitBox, Payload);
			}
		}
	}
}

FString UAnimNotify_HitOneFrame::GetNotifyName_Implementation() const
{
	return TEXT("HitOneFrame");
}

bool UAnimNotify_HitOneFrame::CanTeamAttack(AActor* InAttacker, AActor* InVictim) const
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

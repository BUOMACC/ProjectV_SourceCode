// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_SendGameplayEvent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character/CombatCharacter.h"

UAnimNotifyState_SendGameplayEvent::UAnimNotifyState_SendGameplayEvent()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(153, 153, 255);
#endif

	bUseCollisionCheck = false;
}

FString UAnimNotifyState_SendGameplayEvent::GetNotifyName_Implementation() const
{
	return TEXT("SendGameplayEvent");
}

void UAnimNotifyState_SendGameplayEvent::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
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

void UAnimNotifyState_SendGameplayEvent::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
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

void UAnimNotifyState_SendGameplayEvent::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}

void UAnimNotifyState_SendGameplayEvent::ShapeCheck(ACombatCharacter* CombatCharacter, UCombatAbilitySystemComponent* CombatASC)
{
	if (CombatCharacter == nullptr || CombatASC == nullptr)
	{
		return;
	}

	// Only Server
	if (CombatCharacter->HasAuthority())
	{
		if (bUseCollisionCheck)
		{
			if (Shape == nullptr)
				return;

			TArray<FHitResult> Results;
			Shape->CheckOverlap(CombatCharacter, Results);
			if (Results.Num() > 0)
			{
				// Notify Event To Target
				for (const FHitResult& HitResult : Results)
				{
					FGameplayAbilityTargetDataHandle TargetDataHandle = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(HitResult);
					FGameplayEventData Payload;
					Payload.TargetData = TargetDataHandle;
					Payload.Instigator = CombatCharacter;
					Payload.Target = HitResult.GetActor();
					Payload.ContextHandle = CombatASC->MakeEffectContext();
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitResult.GetActor(), EventTag, Payload);
				}
			}
		}
		else
		{
			FGameplayEventData Payload;
			Payload.Instigator = CombatCharacter;
			Payload.ContextHandle = CombatASC->MakeEffectContext();
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(CombatCharacter, EventTag, Payload);
		}
	}
}
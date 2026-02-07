// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_SendGameplayEvent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character/CombatCharacter.h"
#include "CombatGameplayTags.h"

UAnimNotify_SendGameplayEvent::UAnimNotify_SendGameplayEvent()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(153, 153, 255);
#endif

	bUseCollisionCheck = false;
}

void UAnimNotify_SendGameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

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
		if (bUseCollisionCheck)
		{
			if (Shape == nullptr)
				return;

			TArray<FHitResult> Results;
			Shape->CheckOverlap(MeshComp->GetOwner(), Results);
			if (Results.Num() > 0)
			{
				// Send Gameplay Event To Target
				for (const FHitResult& HitResult : Results)
				{
					FGameplayAbilityTargetDataHandle TargetDataHandle = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(HitResult);
					FGameplayEventData Payload;
					Payload.TargetData = TargetDataHandle;
					Payload.Instigator = CombatCharacter;
					Payload.Target = HitResult.GetActor();
					Payload.ContextHandle = ASC->MakeEffectContext();
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitResult.GetActor(), EventTag, Payload);
				}
			}
		}
		else
		{
			FGameplayEventData Payload;
			Payload.Instigator = CombatCharacter;
			Payload.ContextHandle = ASC->MakeEffectContext();
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(CombatCharacter, EventTag, Payload);
		}
	}
}

FString UAnimNotify_SendGameplayEvent::GetNotifyName_Implementation() const
{
	return TEXT("SendGameplayEvent");
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotifyState_AIDisableAutoGuard.h"
#include "AbilitySystemComponent.h"
#include "CombatGameplayTags.h"

UAnimNotifyState_AIDisableAutoGuard::UAnimNotifyState_AIDisableAutoGuard()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(150, 150, 150);
#endif
}

FString UAnimNotifyState_AIDisableAutoGuard::GetNotifyName_Implementation() const
{
	return FString(TEXT("AI Disable AutoGuard"));
}

void UAnimNotifyState_AIDisableAutoGuard::NotifyBeginAI(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (UAbilitySystemComponent* ASC = GetOwningAICharacterASC(MeshComp))
	{
		ASC->AddLooseGameplayTag(FCombatGameplayTags::Get().Character_AI_DisableAutoGuard, 1);
	}
}

void UAnimNotifyState_AIDisableAutoGuard::NotifyEndAI(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (UAbilitySystemComponent* ASC = GetOwningAICharacterASC(MeshComp))
	{
		ASC->RemoveLooseGameplayTag(FCombatGameplayTags::Get().Character_AI_DisableAutoGuard, 1);
	}
}

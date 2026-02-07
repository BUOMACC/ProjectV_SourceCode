// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_SetCollision.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

UAnimNotify_SetCollision::UAnimNotify_SetCollision()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(52, 235, 58);
#endif
}

FString UAnimNotify_SetCollision::GetNotifyName_Implementation() const
{
	return TEXT("SetCollision");
}

void UAnimNotify_SetCollision::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (!IsValid(Character))
	{
		return;
	}

	if (CollisionSettingType & (1 << (uint8)ECollisionSettingType::Profile))
	{
		Character->GetCapsuleComponent()->SetCollisionProfileName(CollisionProfileName);
	}
	
	if (CollisionSettingType & (1 << (uint8)ECollisionSettingType::EnableType))
	{
		Character->GetCapsuleComponent()->SetCollisionEnabled(CollisionEnabledType);
	}
}

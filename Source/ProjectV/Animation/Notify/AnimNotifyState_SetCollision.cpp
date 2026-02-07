// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_SetCollision.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

UAnimNotifyState_SetCollision::UAnimNotifyState_SetCollision()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(52, 235, 58);
#endif
}

FString UAnimNotifyState_SetCollision::GetNotifyName_Implementation() const
{
	return TEXT("SetCollision");
}

void UAnimNotifyState_SetCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (!IsValid(Character))
	{
		return;
	}

	// Notify가 종료되는 시점에는 아무런 동작이 없겠지만, 문제예방 차원에서 처리
	RestoreIfValid(Character);

	// Add to Restore Data
	FObjectKey Key(Character);
	if (RestoreData.Contains(Key) == false)
	{
		FCollisionSettingsInfo SettingInfo;
		SettingInfo.CollisionProfileName = Character->GetCapsuleComponent()->GetCollisionProfileName();
		SettingInfo.CollisionEnabledType = Character->GetCapsuleComponent()->GetCollisionEnabled();
		RestoreData.Add(Key, SettingInfo);

		if (CollisionSettingType & (1 << (uint8)ECollisionSettingType::Profile))
		{
			Character->GetCapsuleComponent()->SetCollisionProfileName(CollisionProfileName);
		}

		if (CollisionSettingType & (1 << (uint8)ECollisionSettingType::EnableType))
		{
			Character->GetCapsuleComponent()->SetCollisionEnabled(CollisionEnabledType);
		}
	}
}

void UAnimNotifyState_SetCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (!IsValid(Character))
	{
		return;
	}

	RestoreIfValid(Character);

	Super::NotifyEnd(MeshComp, Animation, EventReference);
}

void UAnimNotifyState_SetCollision::RestoreIfValid(ACharacter* Character)
{
	if (!IsValid(Character))
	{
		return;
	}

	FObjectKey Key(Character);
	if (RestoreData.Contains(Key))
	{
		if (CollisionSettingType & (1 << (uint8)ECollisionSettingType::Profile))
		{
			Character->GetCapsuleComponent()->SetCollisionProfileName(RestoreData[Key].CollisionProfileName);
		}

		if (CollisionSettingType & (1 << (uint8)ECollisionSettingType::EnableType))
		{
			Character->GetCapsuleComponent()->SetCollisionEnabled(RestoreData[Key].CollisionEnabledType);
		}

		RestoreData.Remove(Key);
	}
}

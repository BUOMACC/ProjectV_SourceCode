// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Engine/EngineTypes.h"
#include "ProjectVCommonEnums.h"
#include "AnimNotifyState_SetCollision.generated.h"

class ACharacter;

USTRUCT()
struct PROJECTV_API FCollisionSettingsInfo
{
	GENERATED_BODY()

public:
	FName CollisionProfileName;
	ECollisionEnabled::Type CollisionEnabledType;
};

/**
 *	캐릭터의 콜리전 프로필을 수정하는 노티파이 입니다.
 */
UCLASS(meta = (DisplayName = "SetCollision"))
class PROJECTV_API UAnimNotifyState_SetCollision : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UAnimNotifyState_SetCollision();

private:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	void RestoreIfValid(ACharacter* Character);

private:
	UPROPERTY(EditAnywhere, Category = "Collision Settings", meta = (AllowPrivateAccess = "true", Bitmask, BitmaskEnum = "/Script/ProjectV.ECollisionSettingType"))
	uint8 CollisionSettingType = ((1 << (uint32)ECollisionSettingType::Profile));

	UPROPERTY(EditAnywhere, Category = "Collision Settings", meta = (AllowPrivateAccess = "true"))
	FName CollisionProfileName;

	UPROPERTY(EditAnywhere, Category = "Collision Settings", meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<ECollisionEnabled::Type> CollisionEnabledType;

private:
	TMap<FObjectKey, FCollisionSettingsInfo> RestoreData;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ProjectVCommonEnums.h"
#include "AnimNotify_SetCollision.generated.h"

/**
 *  캐릭터의 콜리전 프로필을 영구적으로 수정하는 노티파이 입니다.
 */
UCLASS(meta = (DisplayName = "SetCollision"))
class PROJECTV_API UAnimNotify_SetCollision : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UAnimNotify_SetCollision();

public:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY(EditAnywhere, Category = "Collision Settings", meta = (AllowPrivateAccess = "true", Bitmask, BitmaskEnum = "/Script/ProjectV.ECollisionSettingType"))
	uint8 CollisionSettingType = ((1 << (uint32)ECollisionSettingType::Profile));

	UPROPERTY(EditAnywhere, Category = "Collision Settings", meta = (AllowPrivateAccess = "true"))
	FName CollisionProfileName;

	UPROPERTY(EditAnywhere, Category = "Collision Settings", meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<ECollisionEnabled::Type> CollisionEnabledType;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Data/HitData.h"
#include "Data/OverlappedShapes.h"
#include "AnimNotify_HitOneFrame.generated.h"

/**
 *	한 프레임동안 히트박스를 생성하는 노티파이
 */
UCLASS(meta = (DisplayName = "HitOneFrame"))
class PROJECTV_API UAnimNotify_HitOneFrame : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UAnimNotify_HitOneFrame();

private:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	virtual FString GetNotifyName_Implementation() const override;

	bool CanTeamAttack(AActor* InAttacker, AActor* InVictim) const;

private:
	/* 충돌을 감지할 도형 */
	UPROPERTY(EditAnywhere, Category = "HitOneFrame", Instanced, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UOverlappedShape_Base> Shape;

	/* 데미지 정보 */
	UPROPERTY(EditAnywhere, Category = "HitOneFrame", Instanced, meta = (AllowPrivateAccess = "true"))
	UHitData* HitData;
};

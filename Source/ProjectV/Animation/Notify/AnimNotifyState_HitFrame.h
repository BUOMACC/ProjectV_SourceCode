// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Data/HitData.h"
#include "Data/OverlappedShapes.h"
#include "AnimNotifyState_HitFrame.generated.h"

class ACombatCharacter;
class UCombatAbilitySystemComponent;

UCLASS(meta = (DisplayName = "HitFrame"))
class PROJECTV_API UAnimNotifyState_HitFrame : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UAnimNotifyState_HitFrame();

private:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	void ShapeCheck(ACombatCharacter* CombatCharacter, UCombatAbilitySystemComponent* CombatASC);

	bool CanTeamAttack(AActor* InAttacker, AActor* InVictim) const;

private:
	/* 해당 프레임동안 남아있는지? */
	UPROPERTY(EditAnywhere, Category = "HitOneFrame", meta = (AllowPrivateAccess = "true"))
	bool bUseTick;

	/* 충돌을 감지할 도형 */
	UPROPERTY(EditAnywhere, Category = "HitOneFrame", Instanced, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UOverlappedShape_Base> Shape;

	/* 데미지 정보 */
	UPROPERTY(EditAnywhere, Category = "HitOneFrame", Instanced, meta = (AllowPrivateAccess = "true"))
	UHitData* HitData;
};

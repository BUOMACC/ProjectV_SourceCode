// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Data/OverlappedShapes.h"
#include "GameplayTagContainer.h"
#include "AnimNotifyState_SendGameplayEvent.generated.h"

class ACombatCharacter;
class UCombatAbilitySystemComponent;

/*
*	설정된 타이밍에서 프레임마다 충돌되면 대상에게 이벤트를 전달하는 노티파이
*/
UCLASS(meta = (DisplayName = "SendGameplayEvent Frame"))
class PROJECTV_API UAnimNotifyState_SendGameplayEvent : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UAnimNotifyState_SendGameplayEvent();

private:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	void ShapeCheck(ACombatCharacter* CombatCharacter, UCombatAbilitySystemComponent* CombatASC);

private:
	/* 콜리전 체크를 허용하는지 여부입니다. 비활성화시 대상은 본인이 됩니다. */
	UPROPERTY(EditAnywhere, Category = "Send Gameplay Event", meta = (AllowPrivateAccess = "true"))
	bool bUseCollisionCheck;

	/* 해당 프레임동안 남아있는지? */
	UPROPERTY(EditAnywhere, Category = "EventNotifier", meta = (AllowPrivateAccess = "true", EditCondition = "bUseCollisionCheck == true"))
	bool bUseTick;

	/* 충돌을 감지할 도형 */
	UPROPERTY(EditAnywhere, Category = "EventNotifier", Instanced, meta = (AllowPrivateAccess = "true", EditCondition = "bUseCollisionCheck == true"))
	TObjectPtr<UOverlappedShape_Base> Shape;

	/* 전달할 태그 */
	UPROPERTY(EditAnywhere, Category = "Send Gameplay Event", meta = (AllowPrivateAccess = "true"))
	FGameplayTag EventTag;
};

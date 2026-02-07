// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Data/OverlappedShapes.h"
#include "GameplayTagContainer.h"
#include "AnimNotify_SendGameplayEvent.generated.h"

/**
 *	도형에 충돌된 대상에게 이벤트를 전달하는 노티파이
 */
UCLASS(meta = (DisplayName = "SendGameplayEvent"))
class PROJECTV_API UAnimNotify_SendGameplayEvent : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UAnimNotify_SendGameplayEvent();

private:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	virtual FString GetNotifyName_Implementation() const override;

private:
	/* 콜리전 체크를 허용하는지 여부입니다. 비활성화시 대상은 본인이 됩니다. */
	UPROPERTY(EditAnywhere, Category = "Send Gameplay Event", meta = (AllowPrivateAccess = "true"))
	bool bUseCollisionCheck;

	/* 충돌을 감지할 도형 */
	UPROPERTY(EditAnywhere, Category = "Send Gameplay Event", Instanced, meta = (AllowPrivateAccess = "true", EditCondition = "bUseCollisionCheck == true"))
	TObjectPtr<UOverlappedShape_Base> Shape;

	/* 전달할 태그 */
	UPROPERTY(EditAnywhere, Category = "Send Gameplay Event", meta = (AllowPrivateAccess = "true"))
	FGameplayTag EventTag;
};

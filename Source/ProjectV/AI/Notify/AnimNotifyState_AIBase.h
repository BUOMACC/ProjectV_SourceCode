// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_AIBase.generated.h"

class ACharacter;
class UAbilitySystemComponent;

/**
 *	AI에서 사용하는 전용 노티파이
 *	AI만 동작할수 있도록 체크하는 로직이 추가되었기 때문에
 *	기존의 NotifyBegin(), NotifyTick(), NotifyEnd() 함수는 오버라이딩 불가능
 * 
 *	!! 아래 함수를 오버라이딩하세요. !!
 *	 - NotifyBegin	-> NotifyBeginAI
 *	 - NotifyTick	-> NotifyTickAI
 *	 - NotifyEnd	-> NotifyEndAI
 * 
 */
UCLASS(Abstract, meta = (DisplayName = "AI Base"))
class PROJECTV_API UAnimNotifyState_AIBase : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UAnimNotifyState_AIBase();

private:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override final;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override final;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override final;

	// Override this ↓
protected:
	virtual void NotifyBeginAI(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyTickAI(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyEndAI(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);

protected:
	template<typename T>
	T* GetOwningAI(USkeletalMeshComponent* MeshComp) const
	{
		if (AActor* OwnerActor = MeshComp->GetOwner())
		{
			return Cast<T>(OwnerActor);
		}
		return nullptr;
	}

	UAbilitySystemComponent* GetOwningAICharacterASC(USkeletalMeshComponent* MeshComp) const;

private:
	bool CheckAI(AActor* Actor) const;
};

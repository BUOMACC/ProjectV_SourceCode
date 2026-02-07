// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_Ragdoll.generated.h"

UCLASS(meta = (DisplayName = "Ragdoll"))
class PROJECTV_API UAnimNotify_Ragdoll : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_Ragdoll();

private:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Ragdoll Settings", meta = (AllowPrivateAccess = "true"))
	FVector RandomImpulseMin;

	UPROPERTY(EditAnywhere, Category = "Ragdoll Settings", meta = (AllowPrivateAccess = "true"))
	FVector RandomImpulseMax;
};

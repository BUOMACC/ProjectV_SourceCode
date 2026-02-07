// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifyState_TimedNiagaraEffect.h"
#include "AnimNotifyState_NiagaraTrail.generated.h"

class UNiagaraComponent;

UCLASS()
class PROJECTV_API UAnimNotifyState_NiagaraTrail : public UAnimNotifyState_TimedNiagaraEffect
{
	GENERATED_BODY()
	
public:
	UAnimNotifyState_NiagaraTrail();

public:
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

private:
	void SetTrail(AActor* TargetActor, UNiagaraComponent* NiagaraComponent);

protected:
	UPROPERTY(EditAnywhere, Category = NiagaraSystem)
	FName TrailStartParameterName;

	UPROPERTY(EditAnywhere, Category = NiagaraSystem)
	FName TrailEndParameterName;
};

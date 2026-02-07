// Fill out your copyright notice in the Description page of Project Settings.


#include "HitEffectRow.h"

FHitEffectRow::FHitEffectRow()
{
	ResourceType = EHitEffectResourceType::ParticleSystem;
	HitParticleEffect = nullptr;
	HitNiagaraEffect = nullptr;

	bUseSocketTransform = false;
	RelativeLocation = FVector::ZeroVector;
	RelativeRotation = FRotator::ZeroRotator;
	RelativeScale = FVector::ZeroVector;
	bApplyRandomTransform = false;
	RelativeScale = FVector(1.f, 1.f, 1.f);
}
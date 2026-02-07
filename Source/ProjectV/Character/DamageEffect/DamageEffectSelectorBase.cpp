// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageEffectSelectorBase.h"

UDamageEffectSelectorBase::UDamageEffectSelectorBase()
{
	// ...
}

TSubclassOf<UGameplayEffect> UDamageEffectSelectorBase::ChooseDamageEffectClass_Implementation(FGameplayTag HitMainType, FGameplayTag HitSubType, bool bCanParry, AActor* DamageInstigator, AActor* DamageCauser, AActor* Target) const
{
	return nullptr;
}

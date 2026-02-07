// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatAbilityFunctionLibrary.h"
#include "CombatGameplayTags.h"
#include "GameplayEffect.h"

void UCombatAbilityFunctionLibrary::EffectContextAddHitData(FGameplayEffectContextHandle Handle, UHitData* HitData)
{
	FCombatGameplayEffectContext* CombatContext = static_cast<FCombatGameplayEffectContext*>(Handle.Get());
	if (CombatContext)
	{
		CombatContext->AddHitData(HitData);
	}
}

UHitData* UCombatAbilityFunctionLibrary::EffectContextGetHitData(const FGameplayEffectContextHandle& Handle)
{
	const FGameplayEffectContext* BaseEffectContext = Handle.Get();
	if ((BaseEffectContext != nullptr) && BaseEffectContext->GetScriptStruct()->IsChildOf(FCombatGameplayEffectContext::StaticStruct()))
	{
		const FCombatGameplayEffectContext* CombatEffectContext = static_cast<const FCombatGameplayEffectContext*>(BaseEffectContext);
		return CombatEffectContext->GetHitData();
	}

	return nullptr;
}

FGameplayEffectSpecHandle UCombatAbilityFunctionLibrary::GenerateDamageSpecFromHitData(TSubclassOf<UGameplayEffect> DamageClass, UHitData* HitData, UAbilitySystemComponent* InstigatorASC)
{
	if (!InstigatorASC || !DamageClass || !HitData)
	{
		UE_LOG(LogTemp, Warning, TEXT("[CombatAbilityFunctionLibrary] Cannot Generate Damage Spec."));
		return FGameplayEffectSpecHandle();
	}

	AActor* Instigator = InstigatorASC->GetOwner();
	FGameplayEffectContextHandle ContextHandle = InstigatorASC->MakeEffectContext();

	EffectContextAddHitData(ContextHandle, HitData);

	FGameplayEffectSpecHandle SpecHandle = InstigatorASC->MakeOutgoingSpec(DamageClass, 1, ContextHandle);
	FGameplayEffectSpec* Spec = SpecHandle.Data.Get();
	Spec->SetSetByCallerMagnitude(FCombatGameplayTags::Get().SetByCaller_FixedDamage, HitData->FixedDamage);
	Spec->SetSetByCallerMagnitude(FCombatGameplayTags::Get().SetByCaller_WeaponDamage, 0.f /* TODO: 후에 장착한 장비 공격력 기준으로 Instigator에서 가져오기... */);
	Spec->SetSetByCallerMagnitude(FCombatGameplayTags::Get().SetByCaller_GroggyGauge, HitData->GroggyGauge);
	Spec->SetSetByCallerMagnitude(FCombatGameplayTags::Get().SetByCaller_ParrayGauge, HitData->ParryGauge);

	return SpecHandle;
}

FHitResult UCombatAbilityFunctionLibrary::GetFirstHitResultFromTargetData(const FGameplayAbilityTargetDataHandle& TargetData)
{
	int32 FirstIndex = 0;
	if (TargetData.Data.IsValidIndex(FirstIndex))
	{
		FGameplayAbilityTargetData* Data = TargetData.Data[FirstIndex].Get();
		if (Data)
		{
			const FHitResult* HitResultPtr = Data->GetHitResult();
			if (HitResultPtr)
			{
				return *HitResultPtr;
			}
		}
	}

	return FHitResult();
}

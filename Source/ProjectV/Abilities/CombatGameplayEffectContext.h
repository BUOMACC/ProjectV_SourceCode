// Copyright 2020 Dan Kestranek.

#pragma once

#include "GameplayEffectTypes.h"
#include "HAL/Platform.h"
#include "CombatGameplayEffectContext.generated.h"

class UHitData;

/*
*	기존 EffectContext에 HitData 정보가 추가됨
*/
USTRUCT(BlueprintType)
struct PROJECTV_API FCombatGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	virtual UHitData* GetHitData() const
	{
		return HitData;
	}

	virtual void AddHitData(UHitData* TargetHitData)
	{
		HitData = TargetHitData;
	}

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FCombatGameplayEffectContext::StaticStruct();
	}

	virtual FCombatGameplayEffectContext* Duplicate() const override
	{
		FCombatGameplayEffectContext* NewContext = new FCombatGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		NewContext->HitData = HitData;
		return NewContext;
	}

	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

protected:
	UPROPERTY()
	UHitData* HitData;
};

template<>
struct TStructOpsTypeTraits<FCombatGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FCombatGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true		// Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};

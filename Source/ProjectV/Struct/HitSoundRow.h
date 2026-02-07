// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "HitSoundRow.generated.h"

class USoundBase;

/*
*	피격시 사운드 연출에 관련된 정보를 포함
*	재질별로 어떤 공격에 어떤 사운드 이펙트를 발생할지를 정의한다.
* 
*	Table	: 재질별로 혹은 특수한 몬스터별로 1개씩 존재
*	RowName	: 어떤 타입의 공격을 맞았는지 (**** GameplayTag to Name ****)
*/
USTRUCT(Blueprintable, BlueprintType)
struct PROJECTV_API FHitSoundRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	FHitSoundRow();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USoundBase> HitSoundEffect;
};
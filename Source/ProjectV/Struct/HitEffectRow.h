// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "HitEffectRow.generated.h"

class UParticleSystem;
class UNiagaraSystem;

USTRUCT(BlueprintType)
struct PROJECTV_API FRangedVector
{
	GENERATED_BODY()

	FRangedVector() : MinVector(FVector::ZeroVector), MaxVector(FVector::ZeroVector)
	{}

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector MinVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector MaxVector;
};

USTRUCT(BlueprintType)
struct PROJECTV_API FRangedRotator
{
	GENERATED_BODY()

	FRangedRotator() : MinRotator(FRotator::ZeroRotator), MaxRotator(FRotator::ZeroRotator)
	{}

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FRotator MinRotator;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FRotator MaxRotator;
};

UENUM(BlueprintType)
enum class EHitEffectResourceType : uint8
{
	/* Cascade(Legacy) */
	ParticleSystem,
	Niagara,
};

/*
*	피격시 이펙트 연출에 관련된 정보를 포함
*	재질별로 어떤 공격에 어떤 연출 이펙트를 발생할지를 정의한다.
*	
*	※ 재질별로 나뉘어야 Row 재활용 Table이 생성되는 빈도가 줄어들어 관리에 용이함
*	   RowName은 반드시 GameplayTag(HitEffect)를 Name으로 변환된 값이어야 한다.
*	   대응되는 Effect가 Table Data에 존재하지 않는 경우를 고려해 "Default" Row가 반드시 있어야 한다.
* 
*	Table	: 재질별로 혹은 특수한 몬스터별로 1개씩 존재
*	RowName	: 어떤 타입의 공격을 맞았는지 (**** GameplayTag to Name ****)
*/
USTRUCT(Blueprintable, BlueprintType)
struct PROJECTV_API FHitEffectRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	FHitEffectRow();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EHitEffectResourceType ResourceType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "ResourceType == EHitEffectResourceType::ParticleSystem", EditConditionHides))
	TObjectPtr<UParticleSystem> HitParticleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "ResourceType == EHitEffectResourceType::Niagara", EditConditionHides))
	TObjectPtr<UNiagaraSystem> HitNiagaraEffect;

	/* 소켓정보 사용? ***피격자 기준*** */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bUseSocketTransform;

	/* 이펙트가 소환될 소켓 (미입력시 대상 위치), ***피격자 기준*** */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bUseSocketTransform == true", EditConditionHides))
	FName SocketName;

	/* 이펙트 상대위치 ***공격자 기준*** */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector RelativeLocation;

	/* 이펙트 상대회전 ***공격자 기준*** */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FRotator RelativeRotation;

	/* 이펙트 상대스케일 ***공격자 기준*** */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector RelativeScale;

	/* 랜덤값 적용여부? (설정된 위치에 범위 내 랜덤값만큼 추가적용) ***공격자 기준*** */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bApplyRandomTransform;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bApplyRandomTransform == true", EditConditionHides))
	FRangedVector AdditionalLocationRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bApplyRandomTransform == true", EditConditionHides))
	FRangedRotator AdditionalRotatorRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bApplyRandomTransform == true", EditConditionHides))
	FRangedVector AdditionalScaleRange;
};
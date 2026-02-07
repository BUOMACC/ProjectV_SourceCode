// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatCharacter.h"
#include "DeathBlow/DeathBlowTargetInterface.h"
#include "DeathBlow/DeathBlowInfo.h"
#include "EnemyCharacter.generated.h"

class UDeathBlowExecutorAbility;
class UEnemyAttributeSet;
class UNameplateComponent;
class APatrolPathActor;

UENUM(BlueprintType, meta = (Bitflags))
enum class EEnemyNameplateSettings : uint8
{
	Start			UMETA(ToolTip = "시작시 보여집니다."),
	HealthChanged	UMETA(ToolTip = "체력이 변경되면 보여집니다."),

	Count			UMETA(Hidden)
};

/**
 *	적 캐릭터의 기반이 되는 클래스입니다.
 */
UCLASS()
class PROJECTV_API AEnemyCharacter : public ACombatCharacter, public IDeathBlowTargetInterface
{
	GENERATED_BODY()
	
	friend class AAISpawner;

public:
	AEnemyCharacter();

	virtual void BeginPlay() override;
	virtual void OnAfterDamaged(AActor* DamageInstigator, AActor* DamageSourceActor, const FGameplayEffectSpec& Spec, float Damage) override;

protected:
	virtual void HandleHealthChanged(const FOnAttributeChangeData& ChangeData) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// DeathBlowTarget Interface
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TSubclassOf<UDeathBlowExecutorAbility> GetDeathBlowExecutorAbility(AActor* Executor);
	virtual TSubclassOf<UDeathBlowExecutorAbility> GetDeathBlowExecutorAbility_Implementation(AActor* Executor) override;
	// ~DeathBlowTarget Interface

protected:
	void SetPatrolPathActor(APatrolPathActor* InPatrolPath);

public:
	APatrolPathActor* GetPatrolPathActor() const;

	const FText& GetEnemyDisplayName() const;

protected:
	/* Spawner로 부터 생성된 경우 OnConstruction() 함수 이전에 호출됩니다. */
	UFUNCTION(BlueprintNativeEvent, Category = "Enemy Character")
	void InitializeFromSpawner();
	virtual void InitializeFromSpawner_Implementation();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Setup|Enemy")
	TSubclassOf<UAnimInstance> DefaultLinkedLayerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Setup|Enemy")
	FDeathBlowInfo DeathBlowInfo;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Character Setup|Enemy")
	TObjectPtr<APatrolPathActor> PatrolPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Setup|Enemy")
	FText EnemyDisplayName;

	UPROPERTY(EditAnywhere, Category = "Character Setup|Enemy|Nameplate", meta = (Bitmask, BitmaskEnum = "/Script/ProjectV.EEnemyNameplateSettings"))
	uint8 EnemyNameplateSettings;

	/* EnemyNameplateSettings::HealthChanged 사용시 - 숨겨지기 까지 딜레이입니다. (음수값인 경우 제한없음) */
	UPROPERTY(EditAnywhere, Category = "Character Setup|Enemy|Nameplate")
	float NameplateHideDelay;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UEnemyAttributeSet> EnemyAttribute;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UNameplateComponent> NameplateComponent;

private:
	FTimerHandle NameplateTimerHandle;
};

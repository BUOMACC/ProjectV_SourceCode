// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Abilities/CombatAbilitySystemComponent.h"
#include "Interface/LockonInterface.h"
#include "Interface/DamageInterface.h"
#include "GenericTeamAgentInterface.h"
#include "MotionWarping.h"
#include "MotionWarpingComponent.h"
#include "CombatCharacter.generated.h"

class UCameraComponent;
class UCombatAttributeSet;
class UCombatAbilitySystemComponent;
class UCombatSpringArmComponent;
class UCombatMovementComponent;
class UDataTable;
class UDamageEffectSelectorBase;
class UFootstepData;
class UGrantAbilityData;
class UHealthAttributeSet;
class UHitData;
class USpringArmComponent;
class UUserWidgetComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeChanged, float, OldValue, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDead, ACombatCharacter*, Victim);

/*
 *	FReplicatedAcceleration: Compressed representation of acceleration
 *	 - 압축 후 SimulateProxy에 동기화한 뒤에 압축해제하여 Acceleration 동기화
 */
USTRUCT()
struct FReplicatedAcceleration
{
	GENERATED_BODY()

	UPROPERTY()
	uint8 AccelXYRadians = 0;	// [0, 2*pi]

	UPROPERTY()
	uint8 AccelXYMagnitude = 0;	// [0, MaxAcceleration]

	UPROPERTY()
	int8 AccelZ = 0;
};


/*
*	캐릭터 (적, 플레이어 포함)의 기반이 되는 클래스
*	해당 클래스를 상속받아 PlayerCharacter 혹은 EnemyCharacter로 확장함
*	CharacterBase, CombatCharacter로 통칭해 사용함
*/
UCLASS()
class PROJECTV_API ACombatCharacter : public ACharacter, public IAbilitySystemInterface, public ILockonInterface, public IDamageInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

/*
	######################################
	#			  Events				 #
	######################################
*/
public:
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnAttributeChanged_Health;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnAttributeChanged_MaxHealth;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnAttributeChanged_Stamina;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnAttributeChanged_MaxStamina;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnAttributeChanged_GroggyGauge;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnAttributeChanged_MaxGroggyGauge;

	UPROPERTY(BlueprintAssignable)
	FOnDead OnDead;

/*
	######################################
	#			  Functions				 #
	######################################
*/
public:
	ACombatCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;

	UCombatMovementComponent* GetCombatMovementComponent() const;

	UMotionWarpingComponent* GetMotionWarpingComponent() const;

	// Ability System Interface
	UFUNCTION(BlueprintPure)
	UCombatAbilitySystemComponent* GetCombatAbilitySystemComponent() const;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// ~Ability System Interface

	// Lockon Interface
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UWidgetComponent* GetLockonWidget();
	virtual UWidgetComponent* GetLockonWidget_Implementation() override;
	// ~Lockon Interface

	// Team Agent Interface
	virtual void SetGenericTeamId(const FGenericTeamId& InTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	// ~Team Agent Interface

	/* 캡슐 위치를 고려해 캐릭터의 바닥 좌표를 구합니다. */
	UFUNCTION(BlueprintPure)
	FVector GetFootLocation() const;

	/* 입력한 시간만큼 캐릭터의 월드시간을 조절합니다. */
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void SetTimeDilation(float TimeScale, float Time);

	/* 렉돌 상태로 만듭니다. */
	UFUNCTION(BlueprintCallable)
	void Ragdoll(const FVector& Impulse);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;
	virtual void BindEvents();

	virtual bool CanJumpInternal_Implementation() const override;

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;
	void SetMovementModeTag(EMovementMode MovementMode, uint8 CustomMovementMode, bool bTagEnabled);

	// Damage Interface
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Combat Character")
	void Damage(UAbilitySystemComponent* AttackerASC, const FGameplayAbilityTargetDataHandle& TargetDataHandle, UHitData* HitData);
	virtual void Damage_Implementation(UAbilitySystemComponent* AttackerASC, const FGameplayAbilityTargetDataHandle& TargetDataHandle, UHitData* HitData) override;
	// ~Damage Interface
\
	virtual void SelectAndApplyDamageEffect(UAbilitySystemComponent* AttackerASC, const FGameplayAbilityTargetDataHandle& TargetDataHandle, UHitData* HitData);

	/*
	*	@param DamageInstigator		: 데미지를 입힌 실제 캐릭터
	*	@param DamageSourceActor	: 투사체 등, 데미지를 입힌 물리적 객체 (없으면 nullptr)
	*/
	virtual void OnAfterDamaged(AActor* DamageInstigator, AActor* DamageSourceActor, const FGameplayEffectSpec& Spec, float Damage);
	virtual void OnOutOfHealth(AActor* DamageInstigator, AActor* DamageSourceActor, const FGameplayEffectSpec& Spec, float Damage);

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Combat Character")
	void Dead();
	virtual void Dead_Implementation();

private:
	UFUNCTION()
	void OnRep_ReplicatedAcceleration();

protected:
	virtual void HandleHealthChanged(const FOnAttributeChangeData& ChangeData);
	virtual void HandleMaxHealthChanged(const FOnAttributeChangeData& ChangeData);

	virtual void HandleStaminaChanged(const FOnAttributeChangeData& ChangeData);
	virtual void HandleMaxStaminaChanged(const FOnAttributeChangeData& ChangeData);

	virtual void HandleGroggyGaugeChanged(const FOnAttributeChangeData& ChangeData);
	virtual void HandleMaxGroggyGaugeChanged(const FOnAttributeChangeData& ChangeData);

/*
	######################################
	#			  Variables				 #
	######################################
*/
public:
	/* 피격시 Damage(GE)를 선택하는 규칙을 지정하는 클래스 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Setup")
	TSubclassOf<UDamageEffectSelectorBase> DamageSelectorClass;

	/* 피격시 발생하는 이펙트 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Setup")
	TObjectPtr<UDataTable> HitEffectTable;

	/* 피격시 발생하는 사운드 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Setup")
	TObjectPtr<UDataTable> HitSoundTable;

	/* 걸을때 발생하는 사운드 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Setup")
	TObjectPtr<UFootstepData> FootstepData;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCombatSpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UMotionWarpingComponent> MotionWarp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCombatAbilitySystemComponent> CombatASC;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UHealthAttributeSet> HealthAttribute;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCombatAttributeSet> CombatAttribute;

	/* 초기부여 어빌리티 세트를 지정합니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Setup")
	TArray<UGrantAbilityData*> GrantAbilities;

	UPROPERTY(EditAnywhere, Category = "Character Setup")
	int32 DefaultTeamId;

protected:
	UPROPERTY(Transient, ReplicatedUsing = OnRep_ReplicatedAcceleration)
	FReplicatedAcceleration ReplicatedAcceleration;

	UPROPERTY(Replicated)
	FGenericTeamId MyTeamId;

private:
	FTimerHandle TimeDilationHandle;
};

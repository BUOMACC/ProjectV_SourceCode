// Fill out your copyright notice in the Description page of Project Settings.


#include "AirborneAbility.h"
#include "Abilities/Tasks/AbilityTask_WaitMovementModeChange.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/CharacterMovementComponent.h"

UAirborneAbility::UAirborneAbility()
{
	AbilityTags.AddTag(FCombatGameplayTags::Get().Ability_Type_Hit_Airborne);

	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Character_MainState_Airborne);

	FAbilityTriggerData TriggerEventData;
	TriggerEventData.TriggerTag = FCombatGameplayTags::Get().HitEvent_Result_Airborne;
	TriggerEventData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerEventData);

	Airborne_Start = nullptr;
	Airborne_End = nullptr;
	GroundDetectDelay = 0.25f;
	PlayRate = 1.f;


	FirstDelayTimerHandle.Invalidate();
}

void UAirborneAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, Airborne_Start, PlayRate, NAME_None, true, 1.0f);
	Task->ReadyForActivation();

	// 최소한 공중에 떠있는 시간을 제어하기 위한 딜레이입니다.
	GetWorld()->GetTimerManager().SetTimer(FirstDelayTimerHandle, this, &ThisClass::OnDelay, GroundDetectDelay, false);
}

void UAirborneAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	GetWorld()->GetTimerManager().ClearTimer(FirstDelayTimerHandle);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UAirborneAbility::OnDelay()
{
	if (GetCombatCharacter()->GetCharacterMovement()->IsFalling())
	{
		// 최소한의 시간 이후에도 공중에 있는경우, 지면에 닿는 이벤트를 수신하기 전까지 대기합니다.
		UAbilityTask_WaitMovementModeChange* Task = UAbilityTask_WaitMovementModeChange::CreateWaitMovementModeChange(this, EMovementMode::MOVE_Walking);
		Task->OnChange.AddDynamic(this, &UAirborneAbility::OnMovementModeChanged);
		Task->ReadyForActivation();
	}
	else
	{
		// 이미 바닥인 경우에는, 지면에 닿는 모션을 재생합니다.
		PlayAirborneEndMontage();
	}
}

void UAirborneAbility::OnMovementModeChanged(EMovementMode NewMovementMode)
{
	PlayAirborneEndMontage();
}

void UAirborneAbility::PlayAirborneEndMontage()
{
	// 누워있는 상태를 유지해야 하기 때문에, 어빌리티가 종료되더라도 애니메이션을 끝내면 안됩니다.
	//  ex: 높은 핑에서 누워있는 모션을 유지하지 않으면 대상이 즉시 일어나는 것처럼 보이는 부자연스러운 문제가 발생합니다.
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, Airborne_End, PlayRate, NAME_None, true, 1.0f);
	Task->OnCompleted.AddDynamic(this, &UAirborneAbility::OnAirborneEndCompleted);
	Task->OnInterrupted.AddDynamic(this, &UAirborneAbility::OnAirborneEndInterrupted);
	Task->OnCancelled.AddDynamic(this, &UAirborneAbility::OnAirborneEndCancelled);
	Task->ReadyForActivation();
}

// Airborne End
void UAirborneAbility::OnAirborneEndCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UAirborneAbility::OnAirborneEndInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UAirborneAbility::OnAirborneEndCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
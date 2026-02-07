// Fill out your copyright notice in the Description page of Project Settings.


#include "SmallHitAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "FunctionLibrary/CommonFunctionLibrary.h"
#include "GameFramework/GameState.h"
#include "ProjectVLogs.h"

USmallHitAbility::USmallHitAbility()
{
	AbilityTags.AddTag(FCombatGameplayTags::Get().Ability_Type_Hit_Small);

	ActivationOwnedTags.AddTag(FCombatGameplayTags::Get().Character_MainState_Hit);

	FAbilityTriggerData TriggerEventData;
	TriggerEventData.TriggerTag = FCombatGameplayTags::Get().HitEvent_Result_Small;
	TriggerEventData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerEventData);

	PlayRate = 1.f;
}

void USmallHitAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	/*
	*	[ 피격모션 결정 프로세스 ]
	*		1) 피격 방향에 따른 Montage Set 결정 (앞 / 뒤 / 좌 / 우) 어디서 피격당했는지
	*		2) 결정된 Montage Set에서 공격자의 공격방향에 따른 피격 애니메이션 결정
	*		   ( 공격자가 피격자의 정면에서, 위로 베는 공격을 한 경우 -> 뒤로 젖히는 애니메이션 )
	*/

	UAnimMontage* BestSmallHitMontage = nullptr;
	FHitMontageSet& HitMontageSet = SmallHitMontages[(int32)GetHitDirectionType()];

	if (HitMontageSet.DefaultMontage)
	{
		// * 공격방향에 따른 피격 몽타주를 사용하지 않으면, 기본 몽타주로 사용
		BestSmallHitMontage = HitMontageSet.DefaultMontage;
	}
	else
	{
		// * 공격방향에 따른 피격 몽타주를 사용한 경우, 해당 몽타주를 찾음
		FGameplayTag& AttackDirection = GetHitData()->AttackDirection;
		if (!HitMontageSet.DirectionalMontages.Contains(AttackDirection))
		{
			UE_LOG(LogProjectV, Error, TEXT("[%s]'s HitDirection Montage is not set. [Tag: %s]"), *GetName(), *AttackDirection.ToString());
		}
		else
		{
			FHitMontageRandomList& RandomList = HitMontageSet.DirectionalMontages[AttackDirection];
			BestSmallHitMontage = RandomList.GetRandomMontage(GetWorld()->GetGameState()->GetServerWorldTimeSeconds());
		}
	}

	// * if montage not found
	if (BestSmallHitMontage == nullptr)
	{
		UE_LOG(LogProjectV, Error, TEXT("[%s]'s Montage not found."), *GetName());
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	// * Play Montage
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, BestSmallHitMontage, PlayRate, NAME_None, true, 1.0f);
	Task->OnCompleted.AddDynamic(this, &USmallHitAbility::OnMontageCompleted);
	Task->OnInterrupted.AddDynamic(this, &USmallHitAbility::OnMontageInterrupted);
	Task->OnCancelled.AddDynamic(this, &USmallHitAbility::OnMontageCanceled);
	Task->ReadyForActivation();
}

void USmallHitAbility::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USmallHitAbility::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USmallHitAbility::OnMontageCanceled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

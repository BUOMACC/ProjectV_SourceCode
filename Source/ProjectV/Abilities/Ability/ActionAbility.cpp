// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UActionAbility::UActionAbility()
{
	bEndAbilityWhenAnimationEnd = true;
	bStopWhenAbilityEnd = true;
	PlayRate = 1.f;
}

void UActionAbility::PrePlayAnimation()
{
	K2_PrePlayAnimation();
}

void UActionAbility::PostPlayAnimation()
{
	K2_PostPlayAnimation();
}

UAnimMontage* UActionAbility::GetAnimMontage() const
{
	return K2_GetActionMontage();
}

void UActionAbility::OnMontageBlendingOut()
{
	K2_OnMontageBlendingOut();

	if (!bEndAbilityWhenAnimationEnd)
		return;

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UActionAbility::OnMontageInterrupted()
{
	K2_OnMontageInterrupted();

	if (!bEndAbilityWhenAnimationEnd)
		return;

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UActionAbility::OnMontageEnded()
{
	K2_OnMontageEnded();

	if (!bEndAbilityWhenAnimationEnd)
		return;

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UActionAbility::OnMontageCanceled()
{
	K2_OnMontageCanceled();

	if (!bEndAbilityWhenAnimationEnd)
		return;

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
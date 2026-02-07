// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathBlowExecutorAbility_Default.h"
#include "FunctionLibrary/CommonFunctionLibrary.h"
#include "ProjectVLogs.h"

bool UDeathBlowExecutorAbility_Default::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags) == false)
	{
		return false;
	}

	UObject* SourceObject = GetSourceObject(Handle, ActorInfo);
	if (IsValid(SourceObject) == false)
	{
		UE_LOG(LogProjectV, Error, TEXT("[%s] 어빌리티를 실행할 수 없습니다. SourceObject가 유효하지 않습니다."), *GetName());
		return false;
	}

	ACombatCharacter* SourceActor = Cast<ACombatCharacter>(SourceObject);
	if (IsValid(SourceActor) == false)
	{
		UE_LOG(LogProjectV, Error, TEXT("[%s] 어빌리티를 실행할 수 없습니다. 캐스팅한 SourceObject가 CombatCharacter 타입이 아니거나 유효하지 않습니다."), *GetName());
		return false;
	}

	float Angle = UCommonFunctionLibrary::GetTwoActorAngle(SourceActor, ActorInfo->AvatarActor.Get(), true);
	if (SourceActor->GetAbilitySystemComponent()->HasMatchingGameplayTag(FCombatGameplayTags::Get().Character_MainState_Groggy))
	{
		if (Angle <= 45.f)
		{
			return true;
		}
	}

	return false;
}

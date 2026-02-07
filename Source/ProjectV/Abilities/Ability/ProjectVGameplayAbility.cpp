// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectVGameplayAbility.h"
#include "Player/CombatPlayerController.h"

UProjectVGameplayAbility::UProjectVGameplayAbility()
{
	// * 사용되지 않음 (EpicGames 측에서 코드에서 제거한듯함)
	// ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;

	// 최소한의 정보저장 및 동적 Delegate 바인딩을 위해 사용
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	bAutoTriggerWhenInput = false;
	bAutoActivated = false;
}

ACombatCharacter* UProjectVGameplayAbility::GetCombatCharacter() const
{
	if (CurrentActorInfo)
	{
		return Cast<ACombatCharacter>(CurrentActorInfo->AvatarActor.Get());
	}
	return nullptr;
}

UCharacterMovementComponent* UProjectVGameplayAbility::GetCharacterMovementComponent() const
{
	if (CurrentActorInfo)
	{
		if (ACharacter* Character = Cast<ACharacter>(CurrentActorInfo->AvatarActor))
		{
			return Character->GetCharacterMovement();
		}
	}
	return nullptr;
}

ACombatPlayerController* UProjectVGameplayAbility::GetCombatPlayerController() const
{
	if (CurrentActorInfo)
	{
		return Cast<ACombatPlayerController>(CurrentActorInfo->PlayerController.Get());
	}
	return nullptr;
}

UPlayerComponent* UProjectVGameplayAbility::GetPlayerComponent() const
{
	if (CurrentActorInfo)
	{
		if (APlayerCharacter* Player = Cast<APlayerCharacter>(GetCombatCharacter()))
		{
			return Player->GetPlayerComponent();
		}
	}
	return nullptr;
}

bool UProjectVGameplayAbility::GetLocalInputBuffer(FGameplayTag& OutInputBuffer) const
{
	if (UPlayerComponent* PlayerComp = GetPlayerComponent())
	{
		OutInputBuffer = PlayerComp->GetInputBuffer();
		return true;
	}
	return false;
}

AController* UProjectVGameplayAbility::GetActorController() const
{
	if (CurrentActorInfo)
	{
		if (AController* PC = CurrentActorInfo->PlayerController.Get())
		{
			return PC;
		}

		// Ability에 PlayerController가 설정되어있지 않아 받아오지 못한경우...
		AActor* TestActor = CurrentActorInfo->OwnerActor.Get();
		while (TestActor)
		{
			// 1) OwnerActor가 Controller로 설정된 경우
			if (AController* C = Cast<AController>(TestActor))
			{
				return C;
			}

			// 2) OwnerActor가 CombatCharacter인 경우
			if (ACombatCharacter* CombatCharacter = Cast<ACombatCharacter>(TestActor))
			{
				return CombatCharacter->GetController();
			}

			// 3) 그래도 찾지 못했다면, Owner를 계속 타고들어가 찾음
			//	  - 차량, 말 등의 예외사항 처리
			TestActor = TestActor->GetOwner();
		}
	}
	return nullptr;
}

void UProjectVGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// Clear Inputbuffer when Ability Start
	if (UPlayerComponent* PlayerComp = GetPlayerComponent())
	{
		PlayerComp->ClearInputBuffer();
	}

	// Binding Receive Event
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		FGameplayTagContainer ReceiveEventTag(FCombatGameplayTags::Get().EventParent);
		ReceiveEventCallbackHandle = ASC->AddGameplayEventTagContainerDelegate(ReceiveEventTag, FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &UProjectVGameplayAbility::OnEventReceived));
	}

	// Cache TriggerEventData (Fixed 5.3)
	CurrentEventData = (TriggerEventData != nullptr) ? *TriggerEventData : FGameplayEventData();

	if (bHasBlueprintActivate)
	{
		// A Blueprinted ActivateAbility function must call CommitAbility somewhere in its execution chain.
		K2_ActivateAbility();
	}
	else if (bHasBlueprintActivateFromEvent)
	{
		if (TriggerEventData)
		{
			// A Blueprinted ActivateAbility function must call CommitAbility somewhere in its execution chain.
			K2_ActivateAbilityFromEvent(*TriggerEventData);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Ability %s expects event data but none is being supplied. Use Activate Ability instead of Activate Ability From Event."), *GetName());
			bool bReplicateEndAbility = false;
			bool bWasCancelled = true;
			EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
		}
	}
}

void UProjectVGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	TryActivateAbilityOnSpawn(ActorInfo, Spec);
}

bool UProjectVGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	return bResult;
}

void UProjectVGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// UnBinding Receive Event
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		FGameplayTagContainer ReceiveEventTag(FCombatGameplayTags::Get().EventParent);
		ASC->RemoveGameplayEventTagContainerDelegate(ReceiveEventTag, ReceiveEventCallbackHandle);
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UProjectVGameplayAbility::TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const
{
	const bool bIsPredicting = (Spec.ActivationInfo.ActivationMode == EGameplayAbilityActivationMode::Predicting);

	// Try to activate if auto activated is true.
	if (ActorInfo && !Spec.IsActive() && !bIsPredicting && bAutoActivated)
	{
		UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
		const AActor* AvatarActor = ActorInfo->AvatarActor.Get();

		// If avatar actor is torn off or about to die, don't try to activate until we get the new one.
		if (ASC && AvatarActor && !AvatarActor->GetTearOff() && (AvatarActor->GetLifeSpan() <= 0.0f))
		{
			const bool bIsLocalExecution = (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalPredicted) || (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalOnly);
			const bool bIsServerExecution = (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerOnly) || (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerInitiated);

			const bool bClientShouldActivate = ActorInfo->IsLocallyControlled() && bIsLocalExecution;
			const bool bServerShouldActivate = ActorInfo->IsNetAuthority() && bIsServerExecution;

			if (bClientShouldActivate || bServerShouldActivate)
			{
				ASC->TryActivateAbility(Spec.Handle);
			}
		}
	}
}

void UProjectVGameplayAbility::OnEventReceived(FGameplayTag MatchingTag, const FGameplayEventData* Payload)
{
	FGameplayTag EventTag = FGameplayTag::EmptyTag;
	FGameplayAbilityTargetDataHandle TargetData;
	if (Payload)
	{
		/*
		*	Extract EventTag(각 상황에 따라 받는 태그)
		*/
		EventTag = Payload->EventTag;
		TargetData = Payload->TargetData;
	}

	K2_OnEventReceived(MatchingTag, *Payload);
}
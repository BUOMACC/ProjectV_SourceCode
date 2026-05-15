// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatAbilitySystemComponent.h"
#include "Abilities/Ability/ProjectVGameplayAbility.h"
#include "CombatGameplayTags.h"

UCombatAbilitySystemComponent::UCombatAbilitySystemComponent()
{
	GrantAbilityDataMap.Empty();
}

void UCombatAbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TArray<FGameplayTag> KeysToRemove;
	GrantAbilityDataPendingMap.GetKeys(KeysToRemove);
	for (auto& Elem : GrantAbilityDataPendingMap)
	{
		if (FGrantedAbilityHandles* Handle = GrantAbilityDataPendingMap.Find(Elem.Key))
		{
			if (Handle->CanRemoveNow(this))
			{
				Handle->TakeFromAbilitySystem(this);
				UE_LOG(LogTemp, Log, TEXT("TakeGrantAbilityData( ) Take Success. [TypeTag : %s]"), *Elem.Key.ToString());
			}
			else
			{
				KeysToRemove.Remove(Elem.Key);
			}
		}
	}

	for (const FGameplayTag& Key : KeysToRemove)
	{
		GrantAbilityDataPendingMap.Remove(Key);
	}
}

void UCombatAbilitySystemComponent::AbilityInputPressedTag(const FGameplayTag& InputTag)
{
	if (HasMatchingGameplayTag(FCombatGameplayTags::Get().Character_State_BlockInput))
		return;
	
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			UProjectVGameplayAbility* AbilityCDO = Cast<UProjectVGameplayAbility>(AbilitySpec.Ability);
			if (AbilityCDO && AbilityCDO->InputTag.MatchesTagExact(InputTag))
			{
				AbilityInputPressedHandles.AddUnique(AbilitySpec.Handle);
				AbilityInputHeldHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
}

void UCombatAbilitySystemComponent::AbilityInputReleasedTag(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			UProjectVGameplayAbility* AbilityCDO = Cast<UProjectVGameplayAbility>(AbilitySpec.Ability);
			if (AbilityCDO && AbilityCDO->InputTag.MatchesTagExact(InputTag))
			{
				AbilityInputReleasedHandles.AddUnique(AbilitySpec.Handle);
				AbilityInputHeldHandles.Remove(AbilitySpec.Handle);
			}
		}
	}
}

void UCombatAbilitySystemComponent::ProcessAbilityInput()
{
	TArray<FGameplayAbilitySpecHandle> ReserveActiveAbilities;

	// 1) Pressed
	for (const FGameplayAbilitySpecHandle& Handle : AbilityInputPressedHandles)
	{
		if (FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(Handle))
		{
			if (Spec->Ability == nullptr)
				continue; 

			if (UProjectVGameplayAbility* CombatAbility = Cast<UProjectVGameplayAbility>(Spec->Ability))
			{
				Spec->InputPressed = true;
				if (Spec->IsActive())
				{
					AbilitySpecInputPressed(*Spec);
					continue;
				}

				// AutoTrigger가 true고, DisableFirstCombo 태그가 없어야 실행 예정 목록에 추가
				// 첫 입력을 통해 활성화 되었는지 구분하기 위해 이곳에서 처리
				if (CombatAbility->bAutoTriggerWhenInput && HasMatchingGameplayTag(FCombatGameplayTags::Get().Input_State_DisableFirstCombo) == false)
				{
					ReserveActiveAbilities.AddUnique(Spec->Handle);
				}
			}
		}
	}

	// 2) Activate
	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : ReserveActiveAbilities)
	{
		TryActivateAbility(AbilitySpecHandle);
	}

	// 3) Released
	for (const FGameplayAbilitySpecHandle& Handle : AbilityInputReleasedHandles)
	{
		if (FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(Handle))
		{
			if (Spec->Ability == nullptr)
				continue;

			Spec->InputPressed = false;
			if (Spec->IsActive())
			{
				AbilitySpecInputReleased(*Spec);
			}
		}
	}

	AbilityInputPressedHandles.Reset();
	AbilityInputReleasedHandles.Reset();
}

bool UCombatAbilitySystemComponent::ApplyGrantAbilityData(const UGrantAbilityData* GrantAbilityData, UObject* SourceObject)
{
	if (!IsOwnerActorAuthoritative())
	{
		return false;
	}

	if (GrantAbilityData == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ApplyGrantAbilityData( ) Failed. Ability Data is nullptr."));
		return false;
	}

	const FGameplayTag& TypeTag = GrantAbilityData->TypeTag;
	bool bHasTypeTag = (TypeTag != FGameplayTag::EmptyTag);
	if (!bHasTypeTag)
	{
		UE_LOG(LogTemp, Error, TEXT("ApplyGrantAbilityData( ) Failed. TypeTag is Empty. [%s]"), *GrantAbilityData->GetName());
		return false;
	}

	// 이미 동일한 Key값을 가진값이 존재하면, 회수
	if (IsGrantedAbilityData(TypeTag))
	{
		TakeGrantAbilityData(TypeTag);
	}

	// Create New Handle
	FGrantedAbilityHandles* NewHandle = &GrantAbilityDataMap.Add(TypeTag);
	NewHandle->AbilityTakeRule = GrantAbilityData->AbilityTakeRule;
	
	// Grant Ability
	for (int32 i = 0; i < GrantAbilityData->GrantAbilities.Num(); i++)
	{
		if (GrantAbilityData->GrantAbilities[i] == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("ApplyGrantAbilityData( ) Failed. Ability is nullptr. (Idx : %d)"), i);
			continue;
		}

		// 지금은 1레벨만, 나중에 필요시 레벨추가
		UGameplayAbility* AbilityCDO = GrantAbilityData->GrantAbilities[i]->GetDefaultObject<UGameplayAbility>();
		FGameplayAbilitySpec AbilitySpec(AbilityCDO, 1);
		AbilitySpec.SourceObject = SourceObject;

		const FGameplayAbilitySpecHandle AbilitySpecHandle = GiveAbility(AbilitySpec);
		NewHandle->AddAbilitySpecHandle(AbilitySpecHandle);
	}

	// Grant Gameplay Effect
	for (int32 i = 0; i < GrantAbilityData->GrantGameplayEffects.Num(); i++)
	{
		if (GrantAbilityData->GrantGameplayEffects[i] == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("ApplyGrantAbilityData( ) Failed. GE is nullptr. (Idx : %d)"), i);
			continue;
		}

		// 지금은 1레벨만, 나중에 필요시 레벨추가
		const UGameplayEffect* EffectCDO = GrantAbilityData->GrantGameplayEffects[i]->GetDefaultObject<UGameplayEffect>();
		FGameplayEffectContextHandle Context = MakeEffectContext();

		const FActiveGameplayEffectHandle GameplayEffectHandle = ApplyGameplayEffectToSelf(EffectCDO, 1, Context);
		NewHandle->AddGameplayEffectHandle(GameplayEffectHandle);
	}

	UE_LOG(LogTemp, Log, TEXT("ApplyGrantAbilityData( ) Apply AbilityData Success. [TypeTag : %s]"), *TypeTag.ToString());
	return true;
}

bool UCombatAbilitySystemComponent::TakeGrantAbilityData(const FGameplayTag& GrantAbilityTypeTag)
{
	if (!IsOwnerActorAuthoritative())
	{
		return false;
	}

	if (FGrantedAbilityHandles* Handle = GrantAbilityDataMap.Find(GrantAbilityTypeTag))
	{
		if (Handle->CanRemoveNow(this))
		{
			Handle->TakeFromAbilitySystem(this);
			GrantAbilityDataMap.Remove(GrantAbilityTypeTag);
			UE_LOG(LogTemp, Log, TEXT("TakeGrantAbilityData( ) Take Success. [TypeTag : %s]"), *GrantAbilityTypeTag.ToString());
			return true;
		}
		else
		{
			// Pending 상태로 전환하여 후에 삭제
			GrantAbilityDataPendingMap.Add(GrantAbilityTypeTag, *Handle);
			GrantAbilityDataMap.Remove(GrantAbilityTypeTag);
			UE_LOG(LogTemp, Log, TEXT("TakeGrantAbilityData( ) Handle added to Pending list. [TypeTag : %s]"), *GrantAbilityTypeTag.ToString());
			return true;
		}
	}
	return false;
}

bool UCombatAbilitySystemComponent::IsGrantedAbilityData(const FGameplayTag& GrantAbilityTypeTag) const
{
	return (GrantAbilityDataMap.Contains(GrantAbilityTypeTag));
}

void UCombatAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputPressed(Spec);

	// 어빌리티에 입력을 직접 바인딩한 방식이 아니기때문에, 직접 서버한테 리플리케이트 시켜줘야 함
	// -> 리플리케이트 시켜주면 WaitForInput을 사용할 수 있음
	if (Spec.IsActive())
	{
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void UCombatAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);

	// 어빌리티에 입력을 직접 바인딩한 방식이 아니기때문에, 직접 서버한테 리플리케이트 시켜줘야 함
	// -> 리플리케이트 시켜주면 WaitForInput을 사용할 수 있음
	if (Spec.IsActive())
	{
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void UCombatAbilitySystemComponent::ApplyAbilityBlockAndCancelTags(const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility, bool bEnableBlockTags, const FGameplayTagContainer& BlockTags, bool bExecuteCancelTags, const FGameplayTagContainer& CancelTags)
{
	FGameplayTagContainer LocalBlockTags = BlockTags;
	FGameplayTagContainer LocalCancelTags = CancelTags;

	// ActivationType이 Active인 경우, 실행어빌리티를 하나만 유지하도록 다른 Ability를 캔슬
	if (RequestingAbility->AbilityTags.HasTagExact(FCombatGameplayTags::Get().ActivationType_Active))
	{
		LocalCancelTags.AddTag(FCombatGameplayTags::Get().ActivationType_Active);
	}

	Super::ApplyAbilityBlockAndCancelTags(AbilityTags, RequestingAbility, bEnableBlockTags, LocalBlockTags, bExecuteCancelTags, LocalCancelTags);
}

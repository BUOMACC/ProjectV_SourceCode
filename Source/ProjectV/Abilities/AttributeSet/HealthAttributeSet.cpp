// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthAttributeSet.h"
#include "Abilities/CombatAbilitySystemComponent.h"
#include "CombatGameplayTags.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffectTypes.h"
#include "Net/UnrealNetwork.h"

UHealthAttributeSet::UHealthAttributeSet()
{
	Health = 100.f;
	MaxHealth = 100.f;
}

void UHealthAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthAttributeSet, Health);
	DOREPLIFETIME(UHealthAttributeSet, MaxHealth);
}

void UHealthAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// CurrentValue가 변경될시 Clamping 한다.
	ClampValue(Attribute, NewValue);
}

void UHealthAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

bool UHealthAttributeSet::PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	const FCombatGameplayTags& CombatTags = FCombatGameplayTags::Get();

	FGameplayTagContainer DamageEffectTagContainer;
	Data.EffectSpec.GetAllAssetTags(DamageEffectTagContainer);

	// if Dead? -> Ignore All Gameplay Effect
	if (Data.Target.HasMatchingGameplayTag(CombatTags.Character_MainState_Dead))
	{
		Data.EvaluatedData.Magnitude = 0.f;
		return false;
	}

	// if Invincible? -> Only Allow System Damage
	if (Data.Target.HasMatchingGameplayTag(CombatTags.Character_State_Invincible)
		&& DamageEffectTagContainer.HasTagExact(CombatTags.HitEvent_Result_System) == false)
	{
		Data.EvaluatedData.Magnitude = 0.f;
		return false;
	}

	return true;
}

void UHealthAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetReceivedDamageAttribute())
	{
		const FGameplayEffectContextHandle& Context = Data.EffectSpec.GetEffectContext();
		float DamageToTaken = Data.EvaluatedData.Magnitude;
		float RemainHealth = FMath::Clamp(GetHealth() - DamageToTaken, 0.f, GetMaxHealth());

		SetHealth(RemainHealth);
		SetReceivedDamage(0.f);

		AActor* DamageInstigator = Context.GetInstigator();
		// AActor* DamageCauser = Context.GetEffectCauser();	// same as Instigator(because AvatarActor == OwnerActor)
		AActor* SourceObjectActor = nullptr;					// TODO: if you needed, add projectile actor here.
		if (UObject* SourceObject = Context.GetSourceObject())
		{
			SourceObjectActor = Cast<AActor>(SourceObject);
		}

		// Damage Event
		if (OnDamaged.IsBound())
		{
			// OnDamaged.Broadcast(DamageInstigator, DamageCauser, Data.EffectSpec, DamageToTaken);

			// @see ACombatCharacter::BeginPlay() -> (InitAbilityActorInfo)
			//  OwnerActor와 AvatarActor는 동일하기 때문에, MakeEffectContext(OwnerActor, AvatarActor)로 생성되는
			//	EffectContext에서 Instigator와 Causer가 동일하므로, Instigator와 Causer중 하나만 사용해도됨
			//	 ※ 투사체 등은 필요시 SourceObject로 활용할 예정임
			OnDamaged.Broadcast(DamageInstigator, SourceObjectActor, Data.EffectSpec, DamageToTaken);
		}

		// OutOfHealth Event if Health <= 0
		if (GetHealth() <= 0.f)
		{
			if (OnOutOfHealth.IsBound())
			{
				// OnDead.Broadcast(DamageInstigator, DamageCauser, Data.EffectSpec, DamageToTaken);
				OnOutOfHealth.Broadcast(DamageInstigator, SourceObjectActor, Data.EffectSpec, DamageToTaken);
			}
		}
	}
}

void UHealthAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	// BaseValue가 변경될시 Clamping 한다.
	// Regen같은 경우, Periodic 타입은 Instant(BaseValue)로 적용되므로, 이곳에서도 클램핑 작업이 이루어져야 한다.
	ClampValue(Attribute, NewValue);
}

void UHealthAttributeSet::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
}

void UHealthAttributeSet::ClampValue(const FGameplayAttribute& Attribute, float& Value /* 변경될 값임 (덮어씌워질 값) */) const
{
	if (Attribute == GetHealthAttribute())
	{
		Value = FMath::Clamp(Value, 0.f, GetMaxHealth());
	}
}

void UHealthAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, Health, OldValue);
}

void UHealthAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, MaxHealth, OldValue);
}
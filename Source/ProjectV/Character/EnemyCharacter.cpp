// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "AIController.h"
#include "AI/PatrolPathActor.h"
#include "Abilities/AttributeSet/EnemyAttributeSet.h"
#include "Animation/BaseAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "FunctionLibrary/CommonFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AISense_Damage.h"
#include "UI/Nameplate/NameplateComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	EnemyAttribute = CreateDefaultSubobject<UEnemyAttributeSet>(TEXT("EnemyAttributeSet"));
	NameplateComponent = CreateDefaultSubobject<UNameplateComponent>(TEXT("NameplateComponent"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	EnemyNameplateSettings = 0;
	NameplateHideDelay = 5.f;

	NameplateTimerHandle.Invalidate();
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->LinkAnimClassLayers(DefaultLinkedLayerClass);
	}

	if (EnemyNameplateSettings & (1 << (uint8)EEnemyNameplateSettings::Start))
	{
		NameplateComponent->ShowNameplate(true);
	}
}

void AEnemyCharacter::OnAfterDamaged(AActor* DamageInstigator, AActor* DamageSourceActor, const FGameplayEffectSpec& Spec, float Damage)
{
	Super::OnAfterDamaged(DamageInstigator, DamageSourceActor, Spec, Damage);

	UAISense_Damage::ReportDamageEvent(GetWorld(), this, DamageInstigator, Damage, DamageInstigator->GetActorLocation(), GetActorLocation());
}

void AEnemyCharacter::HandleHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	Super::HandleHealthChanged(ChangeData);

	if (NameplateComponent)
	{
		if (EnemyNameplateSettings & (1 << (uint8)EEnemyNameplateSettings::HealthChanged))
		{
			NameplateComponent->ShowNameplate(true);

			// Nameplate 숨기기 타이머
			if (NameplateTimerHandle.IsValid())
			{
				GetWorldTimerManager().ClearTimer(NameplateTimerHandle);
			}

			GetWorldTimerManager().SetTimer(NameplateTimerHandle, FTimerDelegate::CreateLambda([this]()
				{
					NameplateComponent->ShowNameplate(false);
				}), NameplateHideDelay, false);
		}
	}
}

void AEnemyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (NameplateTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(NameplateTimerHandle);
	}

	Super::EndPlay(EndPlayReason);
}

TSubclassOf<UDeathBlowExecutorAbility> AEnemyCharacter::GetDeathBlowExecutorAbility_Implementation(AActor* Executor)
{
	if (GetAbilitySystemComponent()->HasMatchingGameplayTag(FCombatGameplayTags::Get().Character_MainState_Dead))
	{
		return nullptr;
	}

	if (GetAbilitySystemComponent()->HasMatchingGameplayTag(FCombatGameplayTags::Get().Character_State_DeathBlow))
	{
		return nullptr;
	}

	float Angle = UCommonFunctionLibrary::GetTwoActorAngle(this, Executor, true);

	// Forward
	if (GetAbilitySystemComponent()->HasMatchingGameplayTag(FCombatGameplayTags::Get().Character_MainState_Groggy))
	{
		if (Angle <= 45.f)
		{
			return DeathBlowInfo.DeathBlowAbility_Default;
		}
	}
	// Back
	else if (Angle >= 135.f)
	{
		return DeathBlowInfo.DeathBlowAbility_Back;
	}

	return nullptr;
}

void AEnemyCharacter::SetPatrolPathActor(APatrolPathActor* InPatrolPath)
{
	check(InPatrolPath);
	
	// 이미 있다면 등록해제
	if (PatrolPath)
	{
		PatrolPath->UnregisterActor(this);
		PatrolPath = nullptr;
	}

	// 새로운 순찰 정보를 등록
	PatrolPath = InPatrolPath;
	PatrolPath->RegisterActor(this);
}

APatrolPathActor* AEnemyCharacter::GetPatrolPathActor() const
{
	return PatrolPath;
}

const FText& AEnemyCharacter::GetEnemyDisplayName() const
{
	return EnemyDisplayName;
}

void AEnemyCharacter::InitializeFromSpawner_Implementation()
{
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		AIController->GetBlackboardComponent()->SetValueAsVector(TEXT("HomeLocation"), GetActorLocation());
		AIController->GetBlackboardComponent()->SetValueAsRotator(TEXT("HomeRotation"), GetActorRotation());
	}
}

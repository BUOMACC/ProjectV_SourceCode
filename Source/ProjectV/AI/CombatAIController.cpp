// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"
#include "ProjectVLogs.h"

ACombatAIController::ACombatAIController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception"));
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &ThisClass::OnPerceptionUpdated);

	// AI Sense Config
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AISense_Sight"));
	SightConfig->SightRadius = 1000.f;
	SightConfig->LoseSightRadius = 1000.f;
	SightConfig->SetMaxAge(10.f);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = false;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	PerceptionComponent->ConfigureSense(*SightConfig);

	// AI Hearing Config
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("AISense_Hear"));
	HearingConfig->HearingRange = 500.f;
	HearingConfig->SetMaxAge(1.f);
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = false;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = false;
	PerceptionComponent->ConfigureSense(*HearingConfig);

	// AI Damaged Config
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("AISense_Damage"));
	DamageConfig->SetMaxAge(1.f);
	PerceptionComponent->ConfigureSense(*DamageConfig);

	// Default Values
	bSensingSight = false;
}

void ACombatAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 타겟이 없는 경우에만 시각으로 감각 수치 증가 가능
	if (bSensingSight && !HasTarget())
	{
		float CurrentSenseValue = GetBlackboardComponent()->GetValueAsFloat(TEXT("SenseValue"));
		GetBlackboardComponent()->SetValueAsFloat(TEXT("SenseValue"), CurrentSenseValue + DeltaTime);
	}
}

void ACombatAIController::SetGenericTeamId(const FGenericTeamId& InTeamID)
{
	UE_LOG(LogProjectV, Warning, TEXT("Team Id를 AI Controller에서 설정할 수 없습니다. 캐릭터 내부 함수를 이용하세요."));
}

FGenericTeamId ACombatAIController::GetGenericTeamId() const
{
	if (APawn* ControlledPawn = GetPawn())
	{
		return FGenericTeamId::GetTeamIdentifier(ControlledPawn);
	}
	return FGenericTeamId();
}

void ACombatAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	for (AActor* Actor : UpdatedActors)
	{
		FAIStimulus Stimulus;
		if (CanSenseActor(Actor, EAISenseType::Sight, Stimulus))
		{
			HandleSenseSight(Actor, Stimulus);
		}
		else
		{
			HandleEndSenseSight();
		}

		if (CanSenseActor(Actor, EAISenseType::Hearing, Stimulus))
		{
			HandleSenseHearing(Actor, Stimulus);
		}

		if (CanSenseActor(Actor, EAISenseType::Damaged, Stimulus))
		{
			HandleSenseDamaged(Actor, Stimulus);
		}
	}
}

bool ACombatAIController::CanSenseActor(AActor* Actor, EAISenseType SenseType, FAIStimulus& OutStimulus)
{
	FActorPerceptionBlueprintInfo Info;
	if (PerceptionComponent->GetActorsPerception(Actor, Info))
	{
		for (const FAIStimulus& Stimulus : Info.LastSensedStimuli)
		{
			TSubclassOf<UAISense> SenseClass = UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus);
			TSubclassOf<UAISense> CheckClass;

			switch (SenseType)
			{
			case EAISenseType::Sight:
				CheckClass = UAISense_Sight::StaticClass();
				break;
			case EAISenseType::Hearing:
				CheckClass = UAISense_Hearing::StaticClass();
				break;
			case EAISenseType::Damaged:
				CheckClass = UAISense_Damage::StaticClass();
				break;
			}

			if (SenseClass == CheckClass)
			{
				OutStimulus = Stimulus;
				return Stimulus.WasSuccessfullySensed();
			}
		}
	}

	return false;
}

void ACombatAIController::HandleSenseSight(AActor* Actor, const FAIStimulus& Stimulus)
{
	bSensingSight = true;
	GetBlackboardComponent()->SetValueAsObject(TEXT("LastSenseActor"), Actor);

	HandleSenseSightBP(Actor, Stimulus);
}

void ACombatAIController::HandleEndSenseSight()
{
	bSensingSight = false;

	HandleEndSenseSightBP();
}

void ACombatAIController::HandleSenseHearing(AActor* Actor, const FAIStimulus& Stimulus)
{
	// 타겟이 없는 경우에만 시각으로 감각 수치 증가 가능
	if (!HasTarget())
	{
		GetBlackboardComponent()->SetValueAsObject(TEXT("LastSenseActor"), Actor);

		float CurrentSenseValue = GetBlackboardComponent()->GetValueAsFloat(TEXT("SenseValue"));
		GetBlackboardComponent()->SetValueAsFloat(TEXT("SenseValue"), CurrentSenseValue + Stimulus.Strength);

		HandleSenseHearingBP(Actor, Stimulus);
	}
}

void ACombatAIController::HandleSenseDamaged(AActor* Actor, const FAIStimulus& Stimulus)
{
	GetBlackboardComponent()->SetValueAsObject(TEXT("LastSenseActor"), Actor);

	float CurrentSenseValue = GetBlackboardComponent()->GetValueAsFloat(TEXT("SenseValue"));
	GetBlackboardComponent()->SetValueAsFloat(TEXT("SenseValue"), CurrentSenseValue + 1.f);

	HandleSenseDamagedBP(Actor, Stimulus);
}

bool ACombatAIController::HasTarget() const
{
	UObject* Target = GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor"));
	return IsValid(Target);
}

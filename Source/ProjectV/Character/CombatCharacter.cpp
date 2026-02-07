// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatCharacter.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/AttributeSet/HealthAttributeSet.h"
#include "Abilities/AttributeSet/CombatAttributeSet.h"
#include "Abilities/CombatGameplayEffectContext.h"
#include "CombatGameplayTags.h"
#include "CombatMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Camera/CombatSpringArmComponent.h"
#include "DamageEffect/DamageEffectSelectorBase.h"
#include "Data/GrantAbilityData.h"
#include "FunctionLibrary/CommonFunctionLibrary.h"
#include "FunctionLibrary/CombatAbilityFunctionLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProjectVLogs.h"

ACombatCharacter::ACombatCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCombatMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CombatASC = CreateDefaultSubobject<UCombatAbilitySystemComponent>(TEXT("AbilityComponent"));
	HealthAttribute = CreateDefaultSubobject<UHealthAttributeSet>(TEXT("HealthAttributeSet"));
	CombatAttribute = CreateDefaultSubobject<UCombatAttributeSet>(TEXT("CombatAttributeSet"));

	CameraBoom = CreateDefaultSubobject<UCombatSpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 80.f);
	CameraBoom->TargetArmLength = 280.f;
	CameraBoom->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);
	Camera->bUsePawnControlRotation = false;

	MotionWarp = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarp"));

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Character"));
	GetCapsuleComponent()->SetCapsuleRadius(40.f);

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -92.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
	
	// 기본 정보
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->GravityScale = 1.75f;
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->MaxAcceleration = 3000.f;
	GetCharacterMovement()->BrakingFrictionFactor = 1.f;
	GetCharacterMovement()->BrakingFriction = 0.8f;
	GetCharacterMovement()->bUseSeparateBrakingFriction = true;
	GetCharacterMovement()->GroundFriction = 25.f;
	GetCharacterMovement()->MaxWalkSpeed = 180.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 900.f;
	GetCharacterMovement()->AirControl = 0.75f;

	GetCharacterMovement()->CrouchedHalfHeight = 65.f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 200.f;
	GetCharacterMovement()->bCanWalkOffLedges = true;
	GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	// Bind Events
	HealthAttribute->OnDamaged.AddUObject(this, &ACombatCharacter::OnAfterDamaged);
	HealthAttribute->OnOutOfHealth.AddUObject(this, &ACombatCharacter::OnOutOfHealth);

	// Setup Variables
	HitEffectTable = nullptr;
	HitSoundTable = nullptr;
	FootstepData = nullptr;
	TimeDilationHandle.Invalidate();
}

void ACombatCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ACombatCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACombatCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Initialize ASC (Client && Server)
	// 캐릭터별로 다른 능력치를 가질것이므로 AvatarActor와 OwnerActor를 같게 설정
	CombatASC->InitAbilityActorInfo(this, this);

	// Bind Events
	BindEvents();

	// Set Initial Team
	if (DefaultTeamId != INDEX_NONE)
	{
		FGenericTeamId InitialTeamId(DefaultTeamId);
		SetGenericTeamId(InitialTeamId);
	}
}

void ACombatCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (HasAuthority())
	{
		if (GrantAbilities.Num() > 0)
		{
			for (const UGrantAbilityData* AbilityData : GrantAbilities)
			{
				GetCombatAbilitySystemComponent()->ApplyGrantAbilityData(AbilityData, this);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Character [%s]'s Default Ability is nullptr."), *UKismetSystemLibrary::GetDisplayName(this));
		}
	}
}

UCombatMovementComponent* ACombatCharacter::GetCombatMovementComponent() const
{
	return Cast<UCombatMovementComponent>(GetCharacterMovement());
}

UMotionWarpingComponent* ACombatCharacter::GetMotionWarpingComponent() const
{
	return MotionWarp;
}

void ACombatCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ThisClass, ReplicatedAcceleration, COND_SimulatedOnly);
	DOREPLIFETIME(ThisClass, MyTeamId);
}

void ACombatCharacter::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);

	// Compress Acceleration: XY components as direction + magnitude, Z component as direct value
	if (UCombatMovementComponent* MovementComp = Cast<UCombatMovementComponent>(GetMovementComponent()))
	{
		const double MaxAccel = MovementComp->MaxAcceleration;
		const FVector CurrentAccel = MovementComp->GetCurrentAcceleration();
		double AccelXYRadians, AccelXYMagnitude;
		FMath::CartesianToPolar(CurrentAccel.X, CurrentAccel.Y, AccelXYMagnitude, AccelXYRadians);

		ReplicatedAcceleration.AccelXYRadians = FMath::FloorToInt((AccelXYRadians / TWO_PI) * 255.0);		// [0, 2PI] -> [0, 255]
		ReplicatedAcceleration.AccelXYMagnitude = FMath::FloorToInt((AccelXYMagnitude / MaxAccel) * 255.0);	// [0, MaxAccel] -> [0, 255]
		ReplicatedAcceleration.AccelZ = FMath::FloorToInt((CurrentAccel.Z / MaxAccel) * 127.0);				// [-MaxAccel, MaxAccel] -> [-127, 127]
	}
}

void ACombatCharacter::BindEvents()
{
	CombatASC->GetGameplayAttributeValueChangeDelegate(UHealthAttributeSet::GetHealthAttribute()).AddUObject(this, &ThisClass::HandleHealthChanged);
	CombatASC->GetGameplayAttributeValueChangeDelegate(UHealthAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &ThisClass::HandleMaxHealthChanged);
	CombatASC->GetGameplayAttributeValueChangeDelegate(UCombatAttributeSet::GetStaminaAttribute()).AddUObject(this, &ThisClass::HandleStaminaChanged);
	CombatASC->GetGameplayAttributeValueChangeDelegate(UCombatAttributeSet::GetMaxStaminaAttribute()).AddUObject(this, &ThisClass::HandleMaxStaminaChanged);
	CombatASC->GetGameplayAttributeValueChangeDelegate(UCombatAttributeSet::GetGroggyGaugeAttribute()).AddUObject(this, &ThisClass::HandleGroggyGaugeChanged);
	CombatASC->GetGameplayAttributeValueChangeDelegate(UCombatAttributeSet::GetMaxGroggyGaugeAttribute()).AddUObject(this, &ThisClass::HandleMaxGroggyGaugeChanged);
}

bool ACombatCharacter::CanJumpInternal_Implementation() const
{
	// !bIsCrouching 조건 제거 (앉기 상태에서 점프할 수 있도록)
	return JumpIsAllowedInternal();
}

void ACombatCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	UCharacterMovementComponent* MovementComp = GetCharacterMovement();

	// 추가되었던 Movement Tag 삭제후 새로운 Movement Tag를 추가
	SetMovementModeTag(PrevMovementMode, PreviousCustomMode, false);
	SetMovementModeTag(MovementComp->MovementMode, MovementComp->CustomMovementMode, true);
}

void ACombatCharacter::SetMovementModeTag(EMovementMode MovementMode, uint8 CustomMovementMode, bool bTagEnabled)
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		const FGameplayTag* MovementModeTag = nullptr;
		if (MovementMode == MOVE_Custom)
		{
			// do nothing (never use custom movement mode)
			UE_LOG(LogTemp, Error, TEXT("ACombatCharacter::SetMovementModeTag -> MovementMode is Custom. (not supported)"));
		}
		else
		{
			MovementModeTag = FCombatGameplayTags::Get().MovementModeTagMap.Find(MovementMode);
		}

		if (MovementModeTag && MovementModeTag->IsValid())
		{
			ASC->SetLooseGameplayTagCount(*MovementModeTag, (bTagEnabled ? 1 : 0));
		}
	}
}

void ACombatCharacter::Damage_Implementation(UAbilitySystemComponent* AttackerASC, const FGameplayAbilityTargetDataHandle& TargetDataHandle, UHitData* HitData)
{
	SelectAndApplyDamageEffect(AttackerASC, TargetDataHandle, HitData);
}

void ACombatCharacter::SelectAndApplyDamageEffect(UAbilitySystemComponent* AttackerASC, const FGameplayAbilityTargetDataHandle& TargetDataHandle, UHitData* HitData)
{
	if (HitData == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to Damage. (HitData is nullptr.)"), *UKismetSystemLibrary::GetDisplayName(this));
		return;
	}

	if (DamageSelectorClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Character [%s]'s DamageSelectorClass is nullptr."), *UKismetSystemLibrary::GetDisplayName(this));
		return;
	}

	if (TargetDataHandle.Data.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to Damage. (Target is Empty.)"), *UKismetSystemLibrary::GetDisplayName(this));
		return;
	}

	// Choose Damage Class && Apply
	AActor* DamageInstigator = AttackerASC->GetOwnerActor();
	AActor* DamageCauser = AttackerASC->GetAvatarActor();

	UDamageEffectSelectorBase* DamageSelectorCDO = DamageSelectorClass.GetDefaultObject();
	TSubclassOf<UGameplayEffect> DamageEffectToClass = DamageSelectorCDO->ChooseDamageEffectClass(HitData->HitMainType, HitData->HitSubType, HitData->bCanParry, DamageInstigator, DamageCauser, this);
	if (DamageEffectToClass)
	{
		// HitData, 데미지 등이 설정된 EffectSpec으로 실제 데미지를 적용하는 최종단계
		FGameplayEffectSpecHandle SpecHandle = UCombatAbilityFunctionLibrary::GenerateDamageSpecFromHitData(DamageEffectToClass, HitData, AttackerASC);
		CombatASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void ACombatCharacter::OnAfterDamaged(AActor* DamageInstigator, AActor* DamageSourceActor, const FGameplayEffectSpec& Spec, float Damage)
{
	const FCombatGameplayTags& AllCombatTags = FCombatGameplayTags::Get();

	UE_LOG(LogTemp, Log, TEXT("[CombatCharacter] - Damaged! %f"), Damage);

	// Extract Data
	FGameplayTagContainer DamageEffectTagContainer;
	Spec.GetAllAssetTags(DamageEffectTagContainer);

	FGameplayEventData Payload;
	Payload.ContextHandle = Spec.GetEffectContext();

	UHitData* HitData = UCombatAbilityFunctionLibrary::EffectContextGetHitData(Spec.GetEffectContext());
	ACombatCharacter* Attacker = Cast<ACombatCharacter>(DamageInstigator);

	// Send HitResult Event (Hit React)
	FGameplayTagContainer TagToCheck;
	TagToCheck.AddTag(AllCombatTags.HitEvent_Result_GuardBreak);
	TagToCheck.AddTag(AllCombatTags.HitEvent_Result_Guard);
	TagToCheck.AddTag(AllCombatTags.HitEvent_Result_Small);
	TagToCheck.AddTag(AllCombatTags.HitEvent_Result_Large);
	TagToCheck.AddTag(AllCombatTags.HitEvent_Result_Airborne);
	TagToCheck.AddTag(AllCombatTags.HitEvent_Result_Knockdown);
	TagToCheck.AddTag(AllCombatTags.HitEvent_Result_Falldown);
	TagToCheck.AddTag(AllCombatTags.HitEvent_Result_Parry);
	TagToCheck.AddTag(AllCombatTags.HitEvent_Result_Dummy);

	bool bFoundHitResultTag = false;
	for (auto& It : TagToCheck)
	{
		if (DamageEffectTagContainer.HasTagExact(It))
		{
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, It, Payload);
			bFoundHitResultTag = true;
			break;
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Hit Effect
	if (HitData && Attacker)
	{
		// Apply HitStop to Attacker
		if (Attacker)
		{
			Attacker->SetTimeDilation(HitData->AttackerHitStopScale, HitData->AttackerHitStopTime);
		}

		// Apply HitStop to Victim
		SetTimeDilation(HitData->VictimHitStopScale, HitData->VictimHitStopTime);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////
}

void ACombatCharacter::OnOutOfHealth(AActor* DamageInstigator, AActor* DamageSourceActor, const FGameplayEffectSpec& Spec, float Damage)
{
	const FCombatGameplayTags& AllCombatTags = FCombatGameplayTags::Get();

	FGameplayEventData Payload;
	Payload.ContextHandle = Spec.GetEffectContext();
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, AllCombatTags.Event_OutOfHealth, Payload);
}

void ACombatCharacter::Dead_Implementation()
{
	OnDead.Broadcast(this);
}

FVector ACombatCharacter::GetFootLocation() const
{
	const FVector& CenterLocation = GetActorLocation();
	const FVector& CharacterDownVector = GetActorUpVector() * -1;
	float CapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	return CenterLocation + (CharacterDownVector * CapsuleHalfHeight);
}

void ACombatCharacter::SetTimeDilation_Implementation(float TimeScale, float Time)
{
	if (Time <= 0.f)
	{
		CustomTimeDilation = 1.f;
		return;
	}

	if (TimeDilationHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(TimeDilationHandle);
	}

	CustomTimeDilation = TimeScale;
	GetWorldTimerManager().SetTimer(TimeDilationHandle, FTimerDelegate::CreateLambda([&]()
		{
			CustomTimeDilation = 1.f;
		}), Time, false);
}

void ACombatCharacter::Ragdoll(const FVector& Impulse)
{
	GetCharacterMovement()->Deactivate();

	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetAllBodiesBelowSimulatePhysics(TEXT("Pelvis"), true);
	GetMesh()->AddImpulseToAllBodiesBelow(Impulse, TEXT("Pelvis"), true);
}

void ACombatCharacter::OnRep_ReplicatedAcceleration()
{
	// Decompress Acceleration
	if (UCombatMovementComponent* MovementComp = Cast<UCombatMovementComponent>(GetMovementComponent()))
	{
		const double MaxAccel = MovementComp->MaxAcceleration;
		const double AccelXYMagnitude = double(ReplicatedAcceleration.AccelXYMagnitude) * MaxAccel / 255.0;			// [0, 255] -> [0, MaxAccel]
		const double AccelXYRadians = double(ReplicatedAcceleration.AccelXYRadians) * TWO_PI / 255.0;				// [0, 255] -> [0, 2PI]

		FVector UnpackedAcceleration(FVector::ZeroVector);
		FMath::PolarToCartesian(AccelXYMagnitude, AccelXYRadians, UnpackedAcceleration.X, UnpackedAcceleration.Y);
		UnpackedAcceleration.Z = double(ReplicatedAcceleration.AccelZ) * MaxAccel / 127.0;							// [-127, 127] -> [-MaxAccel, MaxAccel]

		MovementComp->SetReplicatedAcceleration(UnpackedAcceleration);
	}
}

void ACombatCharacter::HandleHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	OnAttributeChanged_Health.Broadcast(ChangeData.OldValue, ChangeData.NewValue);
}

void ACombatCharacter::HandleMaxHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	OnAttributeChanged_MaxHealth.Broadcast(ChangeData.OldValue, ChangeData.NewValue);
}

void ACombatCharacter::HandleStaminaChanged(const FOnAttributeChangeData& ChangeData)
{
	OnAttributeChanged_Stamina.Broadcast(ChangeData.OldValue, ChangeData.NewValue);
}

void ACombatCharacter::HandleMaxStaminaChanged(const FOnAttributeChangeData& ChangeData)
{
	OnAttributeChanged_MaxStamina.Broadcast(ChangeData.OldValue, ChangeData.NewValue);
}

void ACombatCharacter::HandleGroggyGaugeChanged(const FOnAttributeChangeData& ChangeData)
{
	OnAttributeChanged_GroggyGauge.Broadcast(ChangeData.OldValue, ChangeData.NewValue);
}

void ACombatCharacter::HandleMaxGroggyGaugeChanged(const FOnAttributeChangeData& ChangeData)
{
	OnAttributeChanged_MaxGroggyGauge.Broadcast(ChangeData.OldValue, ChangeData.NewValue);
}

UCombatAbilitySystemComponent* ACombatCharacter::GetCombatAbilitySystemComponent() const
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		return Cast<UCombatAbilitySystemComponent>(ASC);
	}
	return nullptr;
}

UAbilitySystemComponent* ACombatCharacter::GetAbilitySystemComponent() const
{
	return CombatASC;
}

UWidgetComponent* ACombatCharacter::GetLockonWidget_Implementation()
{
	UE_LOG(LogProjectV, Error, TEXT("[%s] 캐릭터의 락온 위젯이 설정되어 있지 않습니다."), *GetName());
	return nullptr;
}

void ACombatCharacter::SetGenericTeamId(const FGenericTeamId& InTeamID)
{
	if (HasAuthority())
	{
		MyTeamId = InTeamID;
	}
}

FGenericTeamId ACombatCharacter::GetGenericTeamId() const
{
	return MyTeamId;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTask_ScanNearInteractActor.h"
#include "AbilitySystemComponent.h"
#include "FunctionLibrary/CommonFunctionLibrary.h"
#include "Interface/InteractionInterface.h"
#include "Kismet/KismetMathLibrary.h"

UAbilityTask_ScanNearInteractActor::UAbilityTask_ScanNearInteractActor()
{
	bTickingTask = false;
	bSimulatedTask = false;

	ScanRadius = 100.f;
	ScanAngle = 45.f;
	ScanInterval = 0.1f;
	bShowDebug = false;

	bScanned = false;
}

UAbilityTask_ScanNearInteractActor* UAbilityTask_ScanNearInteractActor::ScanNearInteractActor(UGameplayAbility* OwningAbility, float InScanRadius, float InScanAngle, float InScanInterval, bool bShowDebug)
{
	UAbilityTask_ScanNearInteractActor* MyTask = NewAbilityTask<UAbilityTask_ScanNearInteractActor>(OwningAbility);
	MyTask->ScanRadius = InScanRadius;
	MyTask->ScanAngle = InScanAngle;
	MyTask->ScanInterval = InScanInterval;
	MyTask->bShowDebug = bShowDebug;
	return MyTask;
}

void UAbilityTask_ScanNearInteractActor::Activate()
{
	Super::Activate();

	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().SetTimer(Handle, this, &ThisClass::Scan, ScanInterval, true);
	}
}

void UAbilityTask_ScanNearInteractActor::OnDestroy(bool AbilityIsEnding)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(Handle);
	}

	OnScan.Broadcast(CachedInteractableActor.Get(), CachedInteractableActor.Get());

	Super::OnDestroy(AbilityIsEnding);
}

void UAbilityTask_ScanNearInteractActor::Scan()
{
	UWorld* World = GetWorld();
	AActor* AvatarActor = GetAvatarActor();
	AActor* DesiredActor = nullptr;

	if (World && AvatarActor)
	{
		FCollisionQueryParams Param;
		Param.AddIgnoredActor(AvatarActor);
		Param.bTraceComplex = false;

		TArray<FOverlapResult> Result;
		World->OverlapMultiByChannel(Result, AvatarActor->GetActorLocation(), FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel6, FCollisionShape::MakeSphere(ScanRadius), Param);

		FilterInteractable(AvatarActor, Result);

		// 필터링 이후 적합한 제일 가까운 액터를 가져옴
		if (Result.Num() > 0)
		{
			DesiredActor = Result[0].GetActor();
		}

		// 1) 새로 스캔된 액터가 nullptr인가? -> 더 이상 상호작용할 객체가 없다는 의미이므로 어빌리티 회수 시도
		if (!IsValid(DesiredActor))
		{
			ClearInteractAbility(AvatarActor);
		}
		// 2) 새로 스캔된 다른 액터가 있는가? -> 현재 어빌리티를 회수하고 새로운 어빌리티로 부여
		else if (CachedInteractableActor != DesiredActor)
		{
			ClearInteractAbility(AvatarActor);
			GiveInteractAbility(AvatarActor, DesiredActor);
			
		}

		if (CachedInteractableActor != DesiredActor)
		{
			if (CachedInteractableActor.IsValid() && CachedInteractableActor->Implements<UInteractionInterface>())
			{
				IInteractionInterface::Execute_OnScanEnd(CachedInteractableActor.Get());
			}

			if (IsValid(DesiredActor) && DesiredActor->Implements<UInteractionInterface>())
			{
				IInteractionInterface::Execute_OnScanned(DesiredActor);
				bScanned = true;
			}

			AActor* PrevActor = CachedInteractableActor.Get();
			CachedInteractableActor = DesiredActor;

			OnScan.Broadcast(PrevActor, CachedInteractableActor.Get());
		}

		// 상호작용 액터가 Destroy에 의해 동시에 nullptr이 된 경우, 스캔한 대상이 변경되었는지 감지할 수 없으므로 예외 처리
		if (bScanned && !CachedInteractableActor.IsValid())
		{
			OnScan.Broadcast(nullptr, nullptr);
			bScanned = false;
		}

#if !UE_BUILD_SHIPPING
		if (bShowDebug)
		{
			DrawDebugSphere(GetWorld(), AvatarActor->GetActorLocation(), ScanRadius, 16, FColor::Green, false, ScanInterval);
		}
#endif
	}
}

void UAbilityTask_ScanNearInteractActor::FilterInteractable(AActor* InActor, TArray<FOverlapResult>& Result)
{
	TFunction<bool(const FOverlapResult&)> Filter = [=](const FOverlapResult& Elem)
	{
		// 1) Interaction 인터페이스 상속체크
		if (Elem.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()) == false)
		{
			return true;
		}

		// 2) 허용각도 체크
		float Angle = UCommonFunctionLibrary::GetTwoActorAngle(InActor, Elem.GetActor(), true);
		if (Angle > ScanAngle)
		{
			return true;
		}

		// 3) 상호작용 가능 여부 체크
		if (IInteractionInterface::Execute_CanInteract(Elem.GetActor(), GetOwnerActor(), GetAvatarActor()) == false)
		{
			return true;
		}

		return false;
	};

	Result.RemoveAll(Filter);

	// 거리 낮은순으로 추출
	Result.Sort([=](const FOverlapResult& Lhs, const FOverlapResult& Rhs)
	{
		float Dist_Lhs = FVector::Distance(InActor->GetActorLocation(), Lhs.GetActor()->GetActorLocation());
		float Dist_Rhs = FVector::Distance(InActor->GetActorLocation(), Rhs.GetActor()->GetActorLocation());

		return Dist_Lhs < Dist_Rhs;
	});
}

void UAbilityTask_ScanNearInteractActor::GiveInteractAbility(AActor* AvatarActor, AActor* SourceObject)
{
	if (!IsValid(SourceObject) || !IsValid(AvatarActor))
		return;

	if (AvatarActor->HasAuthority())
	{
		TScriptInterface<IInteractionInterface> Interactable = TScriptInterface<IInteractionInterface>(SourceObject);

		FInteractionInfo Interactioninfo = Interactable->GetInteractionInfo();
		TSubclassOf<UGameplayAbility> AbilityToGrant = Interactioninfo.InteractionAbility;
		check(AbilityToGrant);

		FGameplayAbilitySpec Spec(AbilityToGrant, 1, INDEX_NONE, SourceObject);
		GrantedAbilityHandle = AbilitySystemComponent->GiveAbility(Spec);
	}
}

void UAbilityTask_ScanNearInteractActor::ClearInteractAbility(AActor* AvatarActor)
{
	if (!IsValid(AvatarActor))
		return;

	if (AvatarActor->HasAuthority())
	{
		if (FGameplayAbilitySpec* GrantedSpec = AbilitySystemComponent->FindAbilitySpecFromHandle(GrantedAbilityHandle))
		{
			// 활성화 상태에선 종료 후 회수할 수 있도록 기다림
			if (!GrantedSpec->IsActive())
			{
				AbilitySystemComponent->ClearAbility(GrantedAbilityHandle);
				GrantedAbilityHandle = FGameplayAbilitySpecHandle();
			}
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "NameplateComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "NameplateWidgetInterface.h"
#include "ProjectVLogs.h"

UNameplateComponent::UNameplateComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	NameplateWidget = nullptr;
	DrawSize = FVector2D(100.f, 50.f);
	Offset = FVector(0.f, 0.f, 200.f);
	NameplateWidgetComponent = nullptr;

	bVisible = true;
	bInitialized = false;
}

void UNameplateComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeNameplate();
}

void UNameplateComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (bInitialized && NameplateWidgetComponent)
	{
		if (IsValid(NameplateWidgetComponent->GetWidget()))
		{
			INameplateWidgetInterface::Execute_DeinitializeNameplate(NameplateWidgetComponent->GetWidget());
		}
		NameplateWidgetComponent->DestroyComponent();
	}

	Super::EndPlay(EndPlayReason);
}

void UNameplateComponent::InitializeNameplate()
{
	if (bInitialized)
		return;

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!IsValid(Character))
	{
		UE_LOG(LogNameplate, Error, TEXT("Nameplate Component intiialize failed. owner character is nullptr."));
		bInitialized = false;
		return;
	}

	if (NameplateWidget == nullptr)
	{
		UE_LOG(LogNameplate, Error, TEXT("Nameplate Component intiialize failed. Widget is nullptr."));
		bInitialized = false;
		return;
	}

	if (NameplateWidget->ImplementsInterface(UNameplateWidgetInterface::StaticClass()) == false)
	{
		UE_LOG(LogNameplate, Error, TEXT("Nameplate Component intiialize failed. Widget must be implement NameplateWidget Interface."));
		bInitialized = false;
		return;
	}

	// Initialize
	NameplateWidgetComponent = NewObject<UWidgetComponent>(Character);
	NameplateWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	NameplateWidgetComponent->SetWidgetClass(NameplateWidget);
	NameplateWidgetComponent->SetDrawSize(DrawSize);
	NameplateWidgetComponent->SetupAttachment(Character->GetMesh());
	NameplateWidgetComponent->SetRelativeLocation(Offset);
	NameplateWidgetComponent->RegisterComponent();
	bInitialized = true;

	ShowNameplate(false);

	if (NameplateWidgetComponent->GetWidget())
	{
		INameplateWidgetInterface::Execute_InitializeNameplate(NameplateWidgetComponent->GetWidget(), Character);
	}
}

void UNameplateComponent::ShowNameplate(bool bShowNameplate)
{
	if (!bInitialized)
	{
		return;
	}

	if (bVisible != bShowNameplate)
	{
		bVisible = bShowNameplate;
		NameplateWidgetComponent->SetVisibility(bVisible);
	}
}
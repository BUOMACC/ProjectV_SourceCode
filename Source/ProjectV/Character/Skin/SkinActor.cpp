// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Skin/SkinActor.h"

ASkinActor::ASkinActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASkinActor::BeginPlay()
{
	Super::BeginPlay();
}
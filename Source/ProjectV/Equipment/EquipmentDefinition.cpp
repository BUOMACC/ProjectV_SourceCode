// Copyright Epic Games, Inc. All Rights Reserved.

#include "EquipmentDefinition.h"
#include "EquipmentInstance.h"

UEquipmentDefinition::UEquipmentDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstanceType = UEquipmentInstance::StaticClass();
}


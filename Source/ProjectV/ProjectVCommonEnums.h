// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/*
*	특정한 기능에 종속되지 않는 Enum 집합입니다.
*/

UENUM(BlueprintType)
enum class EDirection : uint8
{
	Forward			UMETA(ToolTip = "↑"),
	Backward		UMETA(ToolTip = "↓"),
	Left			UMETA(ToolTip = "←"),
	Right			UMETA(ToolTip = "→"),
	ForwardLeft		UMETA(ToolTip = "↖"),
	ForwardRight	UMETA(ToolTip = "↗"),
	BackwardLeft	UMETA(ToolTip = "↙"),
	BackwardRight	UMETA(ToolTip = "↘"),
	End				UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EComparisonOperator : uint8
{
	Greater			UMETA(ToolTip = ">"),
	GreaterEqual	UMETA(ToolTip = ">="),
	Less			UMETA(ToolTip = "<"),
	LessEqual		UMETA(ToolTip = "<="),
	Equal			UMETA(ToolTip = "==")
};

UENUM(BlueprintType)
enum class EAISenseType : uint8
{
	None,
	Sight,
	Hearing,
	Damaged,
};

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Peace,
	Patrol,
	Combat,
};

UENUM(BlueprintType, meta = (Bitflags))
enum class ECollisionSettingType : uint8
{
	Profile,
	EnableType,

	Count UMETA(Hidden)
};

ENUM_CLASS_FLAGS(ECollisionSettingType)
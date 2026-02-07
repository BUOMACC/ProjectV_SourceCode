// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CommonFunctionLibrary.generated.h"

/**
 *	자주 사용될만한 기능들을 모아둔 역할을 합니다.
 */
UCLASS()
class PROJECTV_API UCommonFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/* 두 벡터 사이의 랜덤한 값의 벡터를 반환합니다. */
	UFUNCTION(BlueprintPure, Category = "Common FunctionLibrary")
	static FVector RandomVectorRange(const FVector& Vec1, const FVector& Vec2);

	/* 두 회전 사이의 랜덤한 값의 회전을 반환합니다. */
	UFUNCTION(BlueprintPure, Category = "Common FunctionLibrary")
	static FRotator RandomRotatorRange(const FRotator& Rot1, const FRotator& Rot2);

	/* Pivot기준에서 Target과의 각도를 반환합니다. (-180 ~ 180,  bIgnoreSign이 true면 0 ~ 180입니다.) */
	UFUNCTION(BlueprintPure, Category = "Common FunctionLibrary")
	static float GetTwoActorAngle(const AActor* Pivot, const AActor* Target, bool bIgnoreSign = false);

public:
	/*
	*	입력받은 Enum의 이름을 정수값을 이용해 String으로 가져옵니다.
	*	 ex) GetEnumNameString<EWeaponType>(1)
	*/
	template<typename T>
	static const FString& GetEnumNameString(int32 IntegerValue)
	{
		static FString EnumName = "";
		UEnum* Enum = StaticEnum<T>();
		if (Enum)
		{
			EnumName = Enum->GetDisplayNameTextByIndex(IntegerValue).ToString();
		}
		return EnumName;
	}

	/*
	*	입력받은 Enum의 이름을 정수값을 이용해 String으로 가져옵니다.
	*	 ex) GetEnumNameString<EWeaponType>(EWeaponType::Sword)
	*/
	template<typename T>
	static const FString& GetEnumNameString(T EnumValue)
	{
		return GetEnumNameString<T>(static_cast<int32>(EnumValue));
	}

	/*
	*	주어진 TArray의 범위 내에서 랜덤한 인덱스를 반환합니다.
	*	배열이 비어있는 경우 -1을 반환합니다.
	*/
	template<typename T>
	static int32 GetRandomArrayIndex(TArray<T> Array)
	{
		if (Array.Num() == 0)
		{
			return -1;
		}
		return FMath::RandRange(0, Array.Num() - 1);
	}

	/*
	*	주어진 TArray의 범위 내에서 랜덤한 요소에 대한 포인터를 반환합니다.
	*/
	template<typename T>
	static T* GetRandomArrayElement (TArray<T> Array)
	{
		if (Array.Num() == 0)
		{
			return nullptr;
		}
		return &Array[FMath::RandRange(0, Array.Num() - 1)];
	}
};

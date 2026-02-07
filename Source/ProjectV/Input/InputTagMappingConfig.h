// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "InputTagMappingConfig.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FInputActionMappingData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> InputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Input"))
	FGameplayTag InputTag;
};

/*
 *	InputAction을 GameplayTag에 매핑하기 위한 정보
 *	입력 -> Tag에 맞는 액션을 찾아 실행을 위해 사용함
 * 
 *	※ InputBuffer 저장을 위해 순서정의가 필요할 경우, 순서를 맞춰야 합니다.
 *     Ex) 가드를 꾹 누르고있는 상태에서 좌클릭으로 카운터공격... (카운터 공격키가 가드보다 아래에 있어야 인풋버퍼가 덮어씌워짐)
 */
UCLASS()
class PROJECTV_API UInputTagMappingConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	const UInputAction* FindManualInputActionByTag(const FGameplayTag& Tag) const;

public:
	/* 자동으로 하나의 함수에 바인딩되는 입력 (Ability...) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FInputActionMappingData> AutoBindInputActions;

	/* 수동으로 바인딩되는 입력 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FInputActionMappingData> ManualBindInputActions;
};

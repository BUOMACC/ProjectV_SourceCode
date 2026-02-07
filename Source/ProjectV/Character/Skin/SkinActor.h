// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "SkinActor.generated.h"

/*
*	캐릭터의 외형을 표현하는 액터입니다.
*	각 개별 부위가 아닌 전체 완성된 외형을 의미합니다.
* 
*	이 액터를 상속받은 뒤 개별 메시들을 조합해 외형을 만드세요.
*/
UCLASS(BlueprintType, Blueprintable)
class PROJECTV_API ASkinActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ASkinActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void BeginPlay() override;

public:
	FGameplayTag& GetTypeTag() { return TypeTag; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skin Actor", meta = (Categories = "Skin"))
	FGameplayTag TypeTag;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct PROJECTV_API FInteractionInfo
{
	GENERATED_BODY()

public:
	FInteractionInfo()
		: Title(FText::GetEmpty())
		, Desc(FText::GetEmpty())
		, InteractionAbility(nullptr)
	{ }

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FText Desc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	TSubclassOf<UGameplayAbility> InteractionAbility;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTV_API IInteractionInterface
{
	GENERATED_BODY()

public:
	// 상호작용 대상으로 감지될때 호출되는 함수입니다. (Both)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction Interface")
	void OnScanned();

	// 상호작용 대상으로 감지될때 호출되는 함수입니다. (Both)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction Interface")
	void OnScanEnd();

	// 상호작용시 호출되는 함수입니다. (Only Server)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction Interface")
	void OnInteract(AActor* OwnerInstigator /* PC */, AActor* AvatarActor /* Controlled Actor */);

	// 상호작용시 호출되는 함수입니다. (Only Server)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction Interface")
	void OnInteractFinished(AActor* OwnerInstigator /* PC */, AActor* AvatarActor /* Controlled Actor */);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction Interface")
	bool CanInteract(AActor* OwnerInstigator /* PC */, AActor* AvatarActor /* Controlled Actor */) const;

	virtual FInteractionInfo GetInteractionInfo() const = 0;
};

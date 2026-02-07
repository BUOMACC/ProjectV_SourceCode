// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NameplateComponent.generated.h"

class USceneComponent;
class UUserWidget;
class UWidgetComponent;

/*
 *	대상의 이름표를 표시하는 컴포넌트입니다.
 *	동적으로 위젯컴포넌트를 부착하여 표시합니다.
 * 
 *	 - SetPoint(USceneComponent) 함수를 이용해 부착할 위치를 지정합니다.
 *	 - ShowNameplate(bool) 함수로 표시하거나 끌 수 있습니다.
 */
UCLASS(BlueprintType, ClassGroup = (UI), meta = (BlueprintSpawnableComponent))
class PROJECTV_API UNameplateComponent : public UActorComponent
{
	GENERATED_BODY()

/*ㅇ
*	#################################
*	#			Functions			#
*	#################################
*/
public:
	UNameplateComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void InitializeNameplate();

public:
	/* 네임플레이트 위젯을 표시하거나 숨기는 함수입니다. */
	UFUNCTION(BlueprintCallable, Category = "Nameplate Component")
	void ShowNameplate(bool bShowNameplate);

/*
*	#################################
*	#			Variables			#
*	#################################
*/
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Nameplate Component", meta = (MustImplement = "/Script/ProjectV.NameplateWidgetInterface"))
	TSubclassOf<UUserWidget> NameplateWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Nameplate Component")
	FVector2D DrawSize;

	/* 상대적 오프셋입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "Nameplate Component")
	FVector Offset;

private:
	bool bVisible;
	bool bInitialized;

	UPROPERTY()
	TObjectPtr<UWidgetComponent> NameplateWidgetComponent;
};

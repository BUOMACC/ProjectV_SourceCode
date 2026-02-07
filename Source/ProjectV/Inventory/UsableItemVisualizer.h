// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UsableItemVisualizer.generated.h"

class UItemInstance;
class UStaticMeshComponent;

/**
 *	포션 등 사용한 아이템을 시각화하는 액터입니다.
 */
UCLASS()
class PROJECTV_API AUsableItemVisualizer : public AActor
{
	GENERATED_BODY()
	
public:	
	AUsableItemVisualizer();

	void SetVisual(UItemInstance* InItemInstance);

	UStaticMeshComponent* GetVisualMesh() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> VisualMesh;
};

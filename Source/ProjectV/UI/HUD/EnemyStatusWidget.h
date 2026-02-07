// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Nameplate/NameplateWidgetInterface.h"
#include "GameplayEffectTypes.h"
#include "EnemyStatusWidget.generated.h"

class UStatusBarWidget;
class UEnemyLifeWidget;
class UHorizontalBox;
class ACharacter;

/**
 *	몬스터(적)의 머리 위 정보를 표시하는 위젯입니다.
 */
UCLASS()
class PROJECTV_API UEnemyStatusWidget : public UUserWidget, public INameplateWidgetInterface
{
	GENERATED_BODY()
	
public:
	UEnemyStatusWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativePreConstruct() override;

public:
	// Nameplate Widget Interface
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InitializeNameplate(ACharacter* TargetCharacter);
	virtual void InitializeNameplate_Implementation(ACharacter* TargetCharacter) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DeinitializeNameplate();
	virtual void DeinitializeNameplate_Implementation() override;
	// ~Nameplate Widget Interface

protected:
	void OnLifeValueChanged(const FOnAttributeChangeData& ChangeData);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Status Widget")
	TSubclassOf<UEnemyLifeWidget> LifeWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Status Widget")
	int32 LifeWidgetSize;

#if WITH_EDITORONLY_DATA
	/* * 에디터 전용 * 디자인 타임에 보여질 위젯 수를 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Status Widget|Editor")
	int32 PreviewLifeWidgetCount;
#endif

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Enemy Status Widget", meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HB_Life;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy Status Widget", meta = (BindWidget))
	TObjectPtr<UStatusBarWidget> StatusBar_Health;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy Status Widget", meta = (BindWidget))
	TObjectPtr<UStatusBarWidget> StatusBar_Groggy;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy Status Widget")
	TArray<TObjectPtr<UEnemyLifeWidget>> LifeWidgetList;

private:
	FDelegateHandle LifeValueDelegateHandle;
};

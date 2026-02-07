// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayEffectTypes.h"
#include "EnemyHUDWidget.generated.h"

class UStatusBarWidget;
class UEnemyLifeWidget;
class UHorizontalBox;
class UTextBlock;
class AEnemyCharacter;

/**
 *	HUD에서 표시될 적 정보입니다.
 */
UCLASS()
class PROJECTV_API UEnemyHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UEnemyHUDWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void InitWidget(AEnemyCharacter* EnemyCharacter);

protected:
	virtual void NativePreConstruct() override;

protected:
	void OnLifeValueChanged(const FOnAttributeChangeData& ChangeData);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Enemy HUD Widget")
	TSubclassOf<UEnemyLifeWidget> LifeWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy HUD Widget")
	int32 LifeWidgetSize;

#if WITH_EDITORONLY_DATA
	/* * 에디터 전용 * 디자인 타임에 보여질 위젯 수를 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "Enemy HUD Widget|Editor")
	int32 PreviewLifeWidgetCount;
#endif

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Enemy HUD Widget", meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HB_Life;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy HUD Widget", meta = (BindWidget))
	TObjectPtr<UStatusBarWidget> StatusBar_Health;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy HUD Widget", meta = (BindWidget))
	TObjectPtr<UStatusBarWidget> StatusBar_Groggy;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy HUD Widget", meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_EnemyName;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy HUD Widget")
	TArray<TObjectPtr<UEnemyLifeWidget>> LifeWidgetList;

private:
	FDelegateHandle LifeValueDelegateHandle;

	UPROPERTY()
	TObjectPtr<AEnemyCharacter> TargetEnemy;
};

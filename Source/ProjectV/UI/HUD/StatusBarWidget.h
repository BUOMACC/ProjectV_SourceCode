// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayEffectTypes.h"
#include "AttributeSet.h"
#include "StatusBarWidget.generated.h"

class UProgressBar;
class APawn;

/**
 *	캐릭터의 상태바를 표시하는 베이스 위젯입니다.
 */
UCLASS()
class PROJECTV_API UStatusBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UStatusBarWidget(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	void OnCurrentValueStatusChanged(const FOnAttributeChangeData& ChangeData);
	void OnMaxValueStatusChanged(const FOnAttributeChangeData& ChangeData);

public:
	UFUNCTION(BlueprintCallable, Category = "Status Bar Widget")
	void InitializeStatusBar(APawn* InNewPawn);

	UFUNCTION(BlueprintCallable, Category = "Status Bar Widget")
	void UpdateMainProgressBar();

protected:
	/* 가장 메인이되는 프로그래스바로 현재 남은 스테이터스를 표시합니다. (필수) */
	UPROPERTY(BlueprintReadOnly, Category = "Status Bar Widget", meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_Main;

	/* 메인 프로그래스바가 줄어들거나 하는 등의 이유로 연출용으로 사용되는 프로그래스바입니다. (선택) */
	UPROPERTY(BlueprintReadOnly, Category = "Status Bar Widget", meta = (BindWidgetOptional))
	TObjectPtr<UProgressBar> ProgressBar_Back;

	/* 현재값으로 사용할 어트리뷰트입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status Bar Widget")
	FGameplayAttribute CurrentValueAttribute;

	/* 최대값으로 사용할 어트리뷰트입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status Bar Widget")
	FGameplayAttribute MaxValueAttribute;

	/* 어트리뷰트 변화 후 Back 프로그래스바의 애니메이션 시작 딜레이입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status Bar Widget")
	float AnimationDelay;

	/* Back 프로그래스바의 애니메이션 속도입니다. (줄어드는 속도) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status Bar Widget")
	float AnimationInterpSpeed;

private:
	float AnimatedValue;
	float CurrentValue;
	float MaxValue;

	bool bUseAnimation;

	FDelegateHandle CurrentValueStatusDelegateHandle;
	FDelegateHandle MaxValueStatusDelegateHandle;

	FTimerHandle AnimationDelayTimerHandle;

	TWeakObjectPtr<APawn> Pawn;
};

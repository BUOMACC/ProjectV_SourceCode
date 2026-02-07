// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Action/BranchAction_Base.h"
#include "Condition/BranchCondition_Base.h"
#include "GameplayTagContainer.h"
#include "AnimNotifyState_BranchAbility.generated.h"

#pragma region Memo
/*
*	IsNetworked()에서 StandAlone이 아닐때, LocalRole() != SimulatedProxy를 만족하지 않아 false로 반환되는 상황 예시
*	-> SimulatedProxy인 경우이므로, 아래 상황 참고
* 
*		Client에서 실행시
*			-> 본인이 컨트롤중인 캐릭터,				(LocalRole == Autonomous Proxy)	(o)
*			-> 다른 클라이언트가 컨트롤중인 캐릭터,	(LocalRole == Simulated Proxy)	(x)
*			-> AI가 컨트롤중인 캐릭터					(LocalRole == Simulated Proxy)	(x)
*
*		Listen Server에서 실행시
*		  -> 본인이 컨트롤중인 캐릭터,				(LocalRole == Authority)	(o)
*		  -> 다른 클라이언트가 컨트롤중인 캐릭터,		(LocalRole == Authority)	(o)
*		  -> AI가 컨트롤중인 캐릭터					(LocalRole == Authority)	(o)
*
*		Dedicated Server에서 실행시
*			-> 다른 클라이언트가 컨트롤중인 캐릭터		(LocalRole == Authority)	(o)
*			-> AI가 컨트롤중인 캐릭터					(LocalRole == Authority)	(o)
*
*  따라서, Client의 상황에서 다른 플레이어가 컨트롤중이거나 AI가 컨트롤중인 캐릭터를 제외하고 모두 true를 반환
* 
*/
#pragma endregion

UENUM(BlueprintType)
enum class EConditionOperator : uint8
{
	OR,
	AND,
};

UENUM(BlueprintType)
enum class EBranchRule : uint8
{
	OwningClient,
	Server,
};

/**
 *	Condition을 만족하면 특정한 액션을 실행시킵니다.
 */
UCLASS(meta = (DisplayName = "Branch"))
class PROJECTV_API UAnimNotifyState_BranchAbility : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UAnimNotifyState_BranchAbility();

private:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	void ProcessBranchAbility(AActor* Actor);

	bool CheckRule(AActor* Actor) const;
	bool IsNetworked(AActor* Actor) const;
	bool IsClient(AActor* Actor) const;
	bool IsServer(AActor* Actor) const;

private:
	/* 조건만족시 실행할 액션 */
	UPROPERTY(EditAnywhere, Category = "Branch Ability", meta = (AllowPrivateAccess = "true"), Instanced)
	UBranchAction_Base* BranchAction;

	/* ComboEvent를 발생시키기 위한 조건 */
	UPROPERTY(EditAnywhere, Category = "Branch Ability", meta = (AllowPrivateAccess = "true"), Instanced)
	TArray<UBranchCondition_Base*> Conditions;

	/* 조건 체크 연산자 */
	UPROPERTY(EditAnywhere, Category = "Branch Ability", meta = (AllowPrivateAccess = "true"))
	EConditionOperator ConditionOperator;

	/* 어느 환경에서 실행할지 (싱글 플레이면 이 옵션은 무시됨) */
	UPROPERTY(EditAnywhere, Category = "Branch Ability", meta = (AllowPrivateAccess = "true"))
	EBranchRule Rule;
};

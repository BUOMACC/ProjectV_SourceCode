// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectVGameInstance.h"
#include "CombatGameplayTags.h"

void UProjectVGameInstance::Init()
{
	Super::Init();

	// 컴파일시 캐싱된 GameplayTag가 삭제되는 문제가 있어, 시작시 다시 할당해주도록 Init함수에서 처리
	FCombatGameplayTags::Get().InitializeGameplayTags();
}
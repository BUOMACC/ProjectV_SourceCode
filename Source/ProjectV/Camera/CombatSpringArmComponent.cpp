// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSpringArmComponent.h"

void UCombatSpringArmComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	UActorComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 아래 문제 처리
	// - Custom Time Dilation에 의해 카메라의 움직임이 제한되는 현상 처리
	if (GetOwner() && GetOwner()->CustomTimeDilation != 0.f)
	{
		UpdateDesiredArmLocation(bDoCollisionTest, bEnableCameraLag, bEnableCameraRotationLag, DeltaTime / GetOwner()->CustomTimeDilation);
	}
}

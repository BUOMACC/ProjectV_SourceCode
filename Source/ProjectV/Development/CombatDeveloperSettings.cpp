// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatDeveloperSettings.h"

UCombatDeveloperSettings::UCombatDeveloperSettings()
{
	bDisableEnemySpawn = false;
}

FName UCombatDeveloperSettings::GetCategoryName() const
{
	return FApp::GetProjectName();
}
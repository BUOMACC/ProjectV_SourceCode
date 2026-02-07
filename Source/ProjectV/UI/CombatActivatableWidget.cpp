// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatActivatableWidget.h"

TOptional<FUIInputConfig> UCombatActivatableWidget::GetDesiredInputConfig() const
{
	switch (CombatWidgetInputMode)
	{
	case ECombatWidgetInputMode::GameAndUI:
		return FUIInputConfig(ECommonInputMode::All, GameMouseCaptureMode);
	case ECombatWidgetInputMode::GameOnly:
		return FUIInputConfig(ECommonInputMode::Game, GameMouseCaptureMode);
	case ECombatWidgetInputMode::UIOnly:
		return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
	case ECombatWidgetInputMode::Default:
	default:
		return TOptional<FUIInputConfig>();
	}
}
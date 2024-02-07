// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AuraWidgetController.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& InWidgetControlParams)
{
	PlayerController = InWidgetControlParams.PlayerController;
	PlayerState = InWidgetControlParams.PlayerState;
	AbilitySystemComponent = InWidgetControlParams.AbilitySystemComponent;
	AttributeSet = InWidgetControlParams.AttributeSet;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/AuraInputConfig.h"
#include "InputAction.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(FGameplayTag InputTag) const
{
	for(const FAuraInputAction& inputConfig : AbilityInputActions)
	{
		if (inputConfig.InputTag == InputTag)
		{
			return inputConfig.InputAction;
		}
	}

	return nullptr;
}

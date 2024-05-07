// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "AuraInputConfig.h"
#include "AuraEnhancedInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:

	template<class UserClass, typename PressedFuncType, typename HeldFuncType, typename ReleasedFuncType>
	void BindAbilityInputActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, HeldFuncType HeldFunc, ReleasedFuncType ReleasedFunc);
};

template <class UserClass, typename PressedFuncType, typename HeldFuncType, typename ReleasedFuncType>
void UAuraEnhancedInputComponent::BindAbilityInputActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, HeldFuncType HeldFunc, ReleasedFuncType ReleasedFunc)
{
	check(InputConfig);

	for(const FAuraInputAction& inputAction : InputConfig->AbilityInputActions)
	{
		if (inputAction.InputAction && inputAction.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(inputAction.InputAction, ETriggerEvent::Started, Object, PressedFunc, inputAction.InputTag);
			}

			if (HeldFunc)
			{
				BindAction(inputAction.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, inputAction.InputTag);
			}

			if (ReleasedFunc)
			{
				BindAction(inputAction.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, inputAction.InputTag);
			}
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/WidgetController/AuraAttributeMenuWController.h"
#include "AuraAbilitySystemLibrary.generated.h"

enum class ECharacterClass : uint8;
class UAuraOverlayWidgetController;
/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController")
	static UAuraOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController")
	static UAuraAttributeMenuWController* GetAttributeWidgetController(const UObject* WorldContextObject);
	
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 Level, UAbilitySystemComponent* ASC);
	static void InitializeDefaultAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC);
};

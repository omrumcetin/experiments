// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AuraInputConfig.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FAuraInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly)
	const UInputAction* InputAction;
};


UCLASS()
class AURA_API UAuraInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	const UInputAction* FindAbilityInputActionForTag(FGameplayTag InputTag) const;

	UPROPERTY(EditDefaultsOnly, meta=(TitleProperty="InputTag"))
	TArray<FAuraInputAction> AbilityInputActions;
	
};

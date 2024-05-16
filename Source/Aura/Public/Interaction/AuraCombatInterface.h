// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AuraCombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UAuraCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IAuraCombatInterface
{
	GENERATED_BODY()

	
public:
	/** Gets player level */
	virtual int32 GetPlayerLevel() { return 0; }

	virtual FVector GetCombatSocketLocation() { return FVector(); }

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetFacingTarget(const FVector& TargetLocation);
};

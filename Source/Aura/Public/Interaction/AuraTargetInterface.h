// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AuraTargetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAuraTargetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IAuraTargetInterface
{
	GENERATED_BODY()

public:
	virtual void HightlightActor() = 0;
	virtual void UnhighlightActor() = 0;
};

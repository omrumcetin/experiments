// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/AuraCharacterBase.h"
#include "Interaction/AuraTargetInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IAuraTargetInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();
	virtual void BeginPlay() override;

#pragma region TargetInterface
	virtual void HightlightActor() override;
	virtual void UnhighlightActor() override;
#pragma endregion

	virtual uint32 GetPlayerLevel() override { return Level; }

private:
	uint32 Level{1};
};

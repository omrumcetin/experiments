// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

class UGameplayEffect;
class USphereComponent;

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAuraEffectActor();
protected:
	
	UFUNCTION(BlueprintCallable)
	void ApplyGameplayEffectToTarget(AActor* InTargetActor, TSubclassOf<UGameplayEffect> InGameplayEffectClass);
};

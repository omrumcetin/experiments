// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/AuraEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/SphereComponent.h"

// Sets default values
AAuraEffectActor::AAuraEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AAuraEffectActor::ApplyGameplayEffectToTarget(AActor* InTargetActor, const TSubclassOf<UGameplayEffect> InGameplayEffectClass)
{
	UAbilitySystemComponent* targetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(InTargetActor);
	if (!targetASC)
	{
		return;
	}

	FGameplayEffectContextHandle effectContextHandle = targetASC->MakeEffectContext();
	effectContextHandle.AddSourceObject(this);
	
	const FGameplayEffectSpecHandle effectSpecHandle = targetASC->MakeOutgoingSpec(InGameplayEffectClass, 1, effectContextHandle);
	targetASC->ApplyGameplayEffectSpecToSelf(*effectSpecHandle.Data);
}



// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/AuraEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

// Sets default values
AAuraEffectActor::AAuraEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyGameplayEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyGameplayEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyGameplayEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyGameplayEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyGameplayEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyGameplayEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}

	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* targetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);
		if (!::IsValid(targetASC))
		{
			return;
		}

		TArray<FActiveGameplayEffectHandle> handleToRemove;
		for (auto handlePair : ActiveEffectHandles)
		{
			if (targetASC == handlePair.Value)
			{
				targetASC->RemoveActiveGameplayEffect(handlePair.Key);
				handleToRemove.Add(handlePair.Key);
			}
		}

		for (auto& handle : handleToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(handle);
		}
	}
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
	const FActiveGameplayEffectHandle activeEffectHandle = targetASC->ApplyGameplayEffectSpecToSelf(*effectSpecHandle.Data);

	// TObjectPtr -> Get gives Raw Pointer.
	const bool bIsInfinite = effectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(activeEffectHandle, targetASC);
	}
}



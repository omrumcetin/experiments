// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystem/AuraGameplayAbility.h"

void UAuraAbilitySystemComponent::PostAbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &ThisClass::ClientEffectApplied);
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer tagContainer;
	GameplayEffectSpec.GetAllAssetTags(tagContainer);
	OnEffectAssetTagsDelegate.Broadcast(tagContainer);
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for(const TSubclassOf<UGameplayAbility> abilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec abilitySpec {abilityClass, 1};
		if (const UAuraGameplayAbility* auraAbility = Cast<UAuraGameplayAbility>(abilitySpec.Ability))
		{
			abilitySpec.DynamicAbilityTags.AddTag(auraAbility->StartupTag);
			GiveAbility(abilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	for(FGameplayAbilitySpec& abilitySpec : GetActivatableAbilities())
	{
		if (abilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(abilitySpec);
			if (!abilitySpec.IsActive())
			{
				TryActivateAbility(abilitySpec.Handle);
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	for(FGameplayAbilitySpec& abilitySpec : GetActivatableAbilities())
	{
		if (abilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(abilitySpec);
		}
	}
}

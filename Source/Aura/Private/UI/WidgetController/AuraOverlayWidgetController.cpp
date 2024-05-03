// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AuraOverlayWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"

void UAuraOverlayWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet* auraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	OnHealthChanged.Broadcast(auraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(auraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(auraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(auraAttributeSet->GetMaxMana());
}

void UAuraOverlayWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* auraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(auraAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data) -> void
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(auraAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data) -> void
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(auraAttributeSet->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data) -> void
		{
			OnManaChanged.Broadcast(Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(auraAttributeSet->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data) -> void
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		});

	AbilitySystemComponent->OnGameplayEffectAppliedDelegateToSelf.AddLambda(
		[this](UAbilitySystemComponent* InAsc,
			const FGameplayEffectSpec& GameplayEffectSpec,
			FActiveGameplayEffectHandle ActiveGameplayEffectHandle) -> void
		{
			FGameplayTagContainer tagContainer;
			GameplayEffectSpec.GetAllAssetTags(tagContainer);

			for (const FGameplayTag& tag : tagContainer)
			{
				const FMessageContainerRow* row = GetDataTableRowByTag<FMessageContainerRow>(MessageContainerDataTable.Get(), tag );
				if (row)
				{
					OnMessageNotify.Broadcast(*row);
				}
			}
		}
	);
}
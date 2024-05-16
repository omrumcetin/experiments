// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AuraOverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Core/AuraGameplayTags.h"

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
	const UAuraAttributeSet* auraAS = CastChecked<UAuraAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(auraAS->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data) -> void
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(auraAS->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data) -> void
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(auraAS->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data) -> void
		{
			OnManaChanged.Broadcast(Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(auraAS->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data) -> void
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		});
	
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->OnEffectAssetTagsDelegate.AddLambda(
		[this](const FGameplayTagContainer& Container) -> void
		{
			for (const FGameplayTag& tag : Container)
			{
				FGameplayTag messageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if (!tag.MatchesTag(messageTag))
				{
					return;
				}
				const FMessageContainerRow* row = GetDataTableRowByTag<FMessageContainerRow>(MessageContainerDataTable.Get(), tag );
				if (row)
				{
					OnMessageNotify.Broadcast(*row);
				}
			}
		}
	);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AuraAttributeMenuWController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAuraAttributeMenuWController::BindCallbacksToDependencies()
{
	UAuraAttributeSet* auraAS = CastChecked<UAuraAttributeSet>(AttributeSet);
	for(auto& pair : auraAS->TagAttributeMap)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(pair.Value()).AddLambda(
			[this, pair, auraAS](const FOnAttributeChangeData& Data)
			{
				FAuraAttributeInfo info = AttributeInfo->FindAttributeInfoForTag(pair.Key);
				info.AttributeValue = pair.Value().GetNumericValue(auraAS);
				AttributeInfoDelegate.Broadcast(info);
			}
		);
	}
}

void UAuraAttributeMenuWController::BroadcastInitialValues()
{
	UAuraAttributeSet* auraAS = CastChecked<UAuraAttributeSet>(AttributeSet);

	checkf(AttributeInfo, TEXT("Need to assign attribute info in BP."));

	for(auto& pair : auraAS->TagAttributeMap)
	{
		FAuraAttributeInfo info = AttributeInfo->FindAttributeInfoForTag(pair.Key);
		info.AttributeValue = pair.Value().GetNumericValue(auraAS);
		AttributeInfoDelegate.Broadcast(info);
	}
	
}

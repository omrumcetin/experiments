// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AttributeInfo.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& Tag, const bool bLogNotFound) const
{
	for (const FAuraAttributeInfo& info : AttributeInformation)
	{
		if (info.AttributeTag.MatchesTagExact(Tag))
		{
			return info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find info for attribute tag [%s] on AttributeInfo [%s]"), *Tag.ToString(), *GetNameSafe(this));
	}

	return FAuraAttributeInfo();
}

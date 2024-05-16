// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AuraAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "Core/AuraGameplayTags.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine);
	UAuraAssetManager* auraAssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);
	return *auraAssetManager;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FAuraGameplayTags::InitializeNativeGameplayTags();

	// !!!Required to use TARGET DATA!!!
	UAbilitySystemGlobals::Get().InitGlobalData();
}

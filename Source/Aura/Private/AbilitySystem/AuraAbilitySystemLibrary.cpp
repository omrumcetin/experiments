// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Core/AuraGameplayEffectTypes.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/States/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraAttributeMenuWController.h"
#include "UI/WidgetController/AuraWidgetController.h"

UAuraOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	APlayerController* pc = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	AAuraHUD* hud = Cast<AAuraHUD>(pc->GetHUD());
	if (!hud)
	{
		return nullptr;
	}
	AAuraPlayerState* ps = pc->GetPlayerState<AAuraPlayerState>();
	UAbilitySystemComponent* asc = ps->GetAbilitySystemComponent();
	UAttributeSet* as = ps->GetAttributeSet();
	const FWidgetControllerParams wcparams(pc, ps, asc, as);
	return hud->GetOverlayWidgetController(wcparams);
}

UAuraAttributeMenuWController* UAuraAbilitySystemLibrary::GetAttributeWidgetController(const UObject* WorldContextObject)
{
	APlayerController* pc = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	AAuraHUD* hud = Cast<AAuraHUD>(pc->GetHUD());
	if (!hud)
	{
		return nullptr;
	}
	AAuraPlayerState* ps = pc->GetPlayerState<AAuraPlayerState>();
	UAbilitySystemComponent* asc = ps->GetAbilitySystemComponent();
	UAttributeSet* as = ps->GetAttributeSet();
	const FWidgetControllerParams wcparams(pc, ps, asc, as);
	return hud->GetAttributeMenuWidgetController(wcparams);
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,
	ECharacterClass CharacterClass, int32 Level, UAbilitySystemComponent* ASC)
{
	AAuraGameModeBase* gameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!gameModeBase) return;

	UCharacterClassInfo* classInfo = gameModeBase->CharacterClassInfo;

	FGameplayEffectContextHandle effectContext  = ASC->MakeEffectContext();
	effectContext.AddSourceObject(ASC->GetAvatarActor());
	const FGameplayEffectSpecHandle primaryEffectSpec = ASC->MakeOutgoingSpec(classInfo->GetClassDefaultInfo(CharacterClass).PrimaryAttributes, Level, effectContext);
	ASC->ApplyGameplayEffectSpecToSelf(*primaryEffectSpec.Data.Get());
	
	const FGameplayEffectSpecHandle secondaryEffectSpec = ASC->MakeOutgoingSpec(classInfo->SecondaryAttributes, Level, effectContext);
	ASC->ApplyGameplayEffectSpecToSelf(*secondaryEffectSpec.Data.Get());
	
	const FGameplayEffectSpecHandle vitalEffectSpec = ASC->MakeOutgoingSpec(classInfo->VitalAttributes, Level, effectContext);
	ASC->ApplyGameplayEffectSpecToSelf(*vitalEffectSpec.Data.Get());
}

void UAuraAbilitySystemLibrary::InitializeDefaultAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	AAuraGameModeBase* gameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!gameModeBase) return;

	UCharacterClassInfo* classInfo = gameModeBase->CharacterClassInfo;
	for(TSubclassOf<UGameplayAbility> gameplayAbility : classInfo->CommonStartingAbilities)
	{
		FGameplayAbilitySpec abilitySpec {gameplayAbility, 1};
		ASC->GiveAbility(abilitySpec);
	}
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AAuraGameModeBase* gameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!gameModeBase) return nullptr;

	return gameModeBase->CharacterClassInfo;
}

bool UAuraAbilitySystemLibrary::IsBlockingHit(FGameplayEffectContextHandle& ContextHandle)
{
	FAuraGameplayEffectContext* auraContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get());
	return auraContext->IsBlockingHit();
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(FGameplayEffectContextHandle& ContextHandle)
{
	FAuraGameplayEffectContext* auraContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get());
	return auraContext->IsCriticalHit();
}

void UAuraAbilitySystemLibrary::SetIsBlockingHit(FGameplayEffectContextHandle& ContextHandle,const bool bIsBlockingHit)
{
	FAuraGameplayEffectContext* auraContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get());
	auraContext->SetIsBlockingHit(bIsBlockingHit);
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& ContextHandle,const bool bIsCriticalHit)
{
	FAuraGameplayEffectContext* auraContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get());
	auraContext->SetIsCriticalHit(bIsCriticalHit);
}

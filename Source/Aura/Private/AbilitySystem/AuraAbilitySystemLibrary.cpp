// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

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

// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AuraHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/WidgetController/AuraOverlayWidgetController.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "UI/Widgets/AuraUserWidget.h"

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass is not initialized. Please assign it on BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass is not initialized. Please assign it on BP_AuraHUD"));
	
	UUserWidget* widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(widget);

	const FWidgetControllerParams widgetControllerParams(PC, PS, ASC, AS);
	UAuraOverlayWidgetController* auraOverlayWidgetController = GetOverlayWidgetController(widgetControllerParams);
	OverlayWidget->SetWidgetController(auraOverlayWidgetController);
	auraOverlayWidgetController->BroadcastInitialValues();

	widget->AddToViewport(1);
}

UAuraOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& InWidgetControlParams)
{
	if (OverlayWidgetController)
	{
		return OverlayWidgetController;
	}

	OverlayWidgetController = NewObject<UAuraOverlayWidgetController>(this, OverlayWidgetControllerClass);
	OverlayWidgetController->SetWidgetControllerParams(InWidgetControlParams);
	OverlayWidgetController->BindCallbacksToDependencies();

	return OverlayWidgetController;
}

void AAuraHUD::BeginPlay()
{
	Super::BeginPlay();

	
	
}

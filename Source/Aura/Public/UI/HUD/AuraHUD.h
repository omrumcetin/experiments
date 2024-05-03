// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class UAuraAttributeMenuWController;
class UAuraWidgetController;
class UAuraOverlayWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
class UAuraUserWidget;
struct FWidgetControllerParams;
/**
 * 
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;
	
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	UAuraOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& InWidgetControlParams);
	UAuraAttributeMenuWController* GetAttributeMenuWidgetController(const FWidgetControllerParams& InWidgetControlParams);

protected:
	virtual void BeginPlay() override;

private:
	/**
	 * Overlay Widget Controller & Class
	 */
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> OverlayWidgetClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraOverlayWidgetController> OverlayWidgetControllerClass;
	UPROPERTY()
	TObjectPtr<UAuraOverlayWidgetController> OverlayWidgetController;
	/**
	 * Attribute Widget Controller
	 */
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraAttributeMenuWController> AuraAttributeMenuControllerClass;
	UPROPERTY()
	TObjectPtr<UAuraAttributeMenuWController> AuraAttributeMenuController;
};

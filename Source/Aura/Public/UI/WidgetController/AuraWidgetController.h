// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
	:PlayerController(PC)
	,PlayerState(PS)
	,AbilitySystemComponent(ASC)
	,AttributeSet(AS){}

	UPROPERTY(BlueprintReadWrite, Category = WidgetController)
	TObjectPtr<APlayerController> PlayerController{};
	UPROPERTY(BlueprintReadWrite, Category = WidgetController)
	TObjectPtr<APlayerState> PlayerState{};
	UPROPERTY(BlueprintReadWrite, Category = WidgetController)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent{};
	UPROPERTY(BlueprintReadWrite, Category = WidgetController)
	TObjectPtr<UAttributeSet> AttributeSet{};
};

UCLASS()
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& InWidgetControlParams);
	virtual void BroadcastInitialValues() {}
	virtual void BindCallbacksToDependencies() {}

protected:
	// Ability system component
	// Attribute set
	// Player State
	// Player controller

	UPROPERTY(BlueprintReadOnly, Category = WidgetController)
	TObjectPtr<APlayerController> PlayerController;
	UPROPERTY(BlueprintReadOnly, Category = WidgetController)
	TObjectPtr<APlayerState> PlayerState;
	UPROPERTY(BlueprintReadOnly, Category = WidgetController)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(BlueprintReadOnly, Category = WidgetController)
	TObjectPtr<UAttributeSet> AttributeSet;
};

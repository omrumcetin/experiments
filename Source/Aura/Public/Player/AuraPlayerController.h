// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UAuraAbilitySystemComponent;
class UAuraInputConfig;
class IAuraTargetInterface;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
struct FGameplayTag;
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();

	virtual void PlayerTick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputMappingContext> AuraContext;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UAuraInputConfig* InputConfig;
	
	UPROPERTY()
	UAuraAbilitySystemComponent* AuraAbilitySystemComponent;

	UFUNCTION()
	void AbilityInputPressed(FGameplayTag InputTag);

	UFUNCTION()
	void AbilityInputHeld(FGameplayTag InputTag);

	UFUNCTION()
	void AbilityInputReleased(FGameplayTag InputTag);

	void Move(const FInputActionValue& InputActionValue);
	void CursorTrace();

	UAuraAbilitySystemComponent* GetASC();

	IAuraTargetInterface* LastActor;
	IAuraTargetInterface* CurrentActor;
};

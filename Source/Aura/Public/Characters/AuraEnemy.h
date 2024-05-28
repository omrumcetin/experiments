// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Characters/AuraCharacterBase.h"
#include "Interaction/AuraTargetInterface.h"
#include "UI/WidgetController/AuraOverlayWidgetController.h"
#include "AuraEnemy.generated.h"

enum class ECharacterClass : uint8;
class UAuraEnemyWidgetController;
class UAuraWidgetController;
class UAuraUserWidget;
class UWidgetComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IAuraTargetInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;

#pragma region TargetInterface
	virtual void HightlightActor() override;
	virtual void UnhighlightActor() override;
#pragma endregion

	virtual int32 GetPlayerLevel() const override { return Level; }

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

protected:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> OverheadWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level{1};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass{ECharacterClass::Melee};

	UPROPERTY()
	float BaseWalkSpeed{100.f};
};

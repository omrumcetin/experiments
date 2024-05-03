// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/AuraCombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public IAuraCombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAuraCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category=Combat)
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributesEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributesEffectClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributesEffectClass;

	virtual void InitAbilityActorInfo() {}
	void InitializePrimaryAttributes() const;
	void InitializeSecondaryAttributes() const;
	void InitializeVitalAttributes() const;
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, uint32 Level) const;
};

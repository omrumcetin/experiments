// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AuraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties() = default;
	
	FGameplayEffectContextHandle ContextHandle;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceASC{};

	UPROPERTY()
	TObjectPtr<AActor> SourceAvatarActor{};

	UPROPERTY()
	TObjectPtr<AController> SourceController{};

	UPROPERTY()
	TObjectPtr<ACharacter> SourceCharacter{};
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetASC{};

	UPROPERTY()
	TObjectPtr<AActor> TargetAvatarActor{};

	UPROPERTY()
	TObjectPtr<AController> TargetController{};

	UPROPERTY()
	TObjectPtr<ACharacter> TargetCharacter{};
};

UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UAuraAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	//-------------------- RPG ATTRIBUTES -----------------------------

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Attributes|Primary")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(ThisClass, Strength);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Attribute|Primary")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(ThisClass, Intelligence);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "Attribute|Primary")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(ThisClass, Vigor);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilliance, Category = "Attribute|Primary")
	FGameplayAttributeData Resilliance;
	ATTRIBUTE_ACCESSORS(ThisClass, Resilliance);

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;
	
	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;

	UFUNCTION()
	void OnRep_Resilliance(const FGameplayAttributeData& OldResilliance) const;

	//-------------------- VITAL ATTRIBUTES ---------------------------
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Attributes|Vital")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(ThisClass, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Attribute|Vital")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(ThisClass, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Attribute|Vital")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(ThisClass, Mana);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Attribute|Vital")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(ThisClass, MaxMana);
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxHealth) const;

	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& EffectProperties);
};



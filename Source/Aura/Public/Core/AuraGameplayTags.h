// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
/**
 * AuraGameplayTags
 * Singleton containing native Gameplay Tags
 */
struct FAuraGameplayTags
{
	static void InitializeNativeGameplayTags();
	static const FAuraGameplayTags& Get() { return GameplayTags; }

	// ------------------------------ Gameplay Tags --------------------------
	/** Attributes */
	/**** Primary */
	FGameplayTag AttributesPrimaryIntelligence;
	FGameplayTag AttributesPrimaryResiliance;
	FGameplayTag AttributesPrimaryStrength;
	FGameplayTag AttributesPrimaryVigor;
	/**** Secondary */
	FGameplayTag AttributesSecondaryArmor;
	FGameplayTag AttributesSecondaryArmorPenetration;
	FGameplayTag AttributesSecondaryCriticalHitChance;
	FGameplayTag AttributesSecondaryCriticalHitDamage;
	FGameplayTag AttributesSecondaryCriticalHitResistance;
	FGameplayTag AttributesSecondaryBlockChance;
	FGameplayTag AttributesSecondaryRegenHealth;
	FGameplayTag AttributesSecondaryRegenMana;
	
	/**** Vital */
	FGameplayTag AttributesVitalHealth;
	FGameplayTag AttributesVitalMaxHealth;
	FGameplayTag AttributesVitalMana;
	FGameplayTag AttributesVitalMaxMana;
	
	/** Message */
	FGameplayTag MessageHealthPotion;
	FGameplayTag MessageHealthCrystal;
	FGameplayTag MessageManaPotion;
	FGameplayTag MessageManaCrystal;
	// ------------------------------ Gameplay Tags --------------------------

private:
	static FAuraGameplayTags GameplayTags;
	void AddAllTags();
	void AddTag(FGameplayTag& GameplayTag,const ANSICHAR* TagName, const ANSICHAR* TagComment);
};
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
	FGameplayTag AttributesSecondaryMaxHealth;
	FGameplayTag AttributesSecondaryMaxMana;
	
	/**** Vital */
	FGameplayTag AttributesVitalHealth;
	FGameplayTag AttributesVitalMana;

	/** Inputs */
	FGameplayTag InputTagRightMouseButton;
	FGameplayTag InputTagLeftMouseButton;
	FGameplayTag InputTagAction1;
	FGameplayTag InputTagAction2;
	FGameplayTag InputTagAction3;
	FGameplayTag InputTagAction4;

	/** Data */
	FGameplayTag DataDamage;

	/** Effects */
	FGameplayTag EffectHitReact;
	
	
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
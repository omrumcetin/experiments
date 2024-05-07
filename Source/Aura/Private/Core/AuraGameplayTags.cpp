// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AuraGameplayTags.h"

#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	GameplayTags.AddAllTags();
}

void FAuraGameplayTags::AddAllTags()
{
	AddTag(AttributesPrimaryIntelligence, "Attributes.Primary.Intelligence", "Increases magic damage");
	AddTag(AttributesPrimaryResiliance, "Attributes.Primary.Resiliance", "Increases magic defense");
	AddTag(AttributesPrimaryStrength, "Attributes.Primary.Strength", "Increases physical damage");
	AddTag(AttributesPrimaryVigor, "Attributes.Primary.Vigor", "Increases durability");
	
	AddTag(AttributesSecondaryArmor, "Attributes.Secondary.Armor", "Increases magic damage");
	AddTag(AttributesSecondaryArmorPenetration, "Attributes.Secondary.ArmorPenetration", "Increases true physical damage");
	AddTag(AttributesSecondaryCriticalHitChance, "Attributes.Secondary.CriticalHitChance", "Increases critical hit chance");
	AddTag(AttributesSecondaryCriticalHitDamage, "Attributes.Secondary.CriticalHitDamage", "Increases critical hit damage");
	AddTag(AttributesSecondaryCriticalHitResistance, "Attributes.Secondary.CriticalHitResistance", "Increases critical hit resistance");
	AddTag(AttributesSecondaryBlockChance, "Attributes.Secondary.BlockChance", "Increases blocking chance");
	AddTag(AttributesSecondaryRegenHealth, "Attributes.Secondary.Regen.Health", "Regen amount of health in 1s");
	AddTag(AttributesSecondaryRegenMana, "Attributes.Secondary.Regen.Mana", "Regen amount of mana in 1s");

	AddTag(AttributesVitalHealth, "Attributes.Vital.Health", "Health of character");
	AddTag(AttributesVitalMaxHealth, "Attributes.Vital.MaxHealth", "Maximum amount of health that character can have");
	AddTag(AttributesVitalMana, "Attributes.Vital.Mana", "Current mana of character");
	AddTag(AttributesVitalMaxMana, "Attributes.Vital.MaxMana", "Maximum amount of mana that character can have");

	AddTag(InputTagRightMouseButton, "InputTag.RightMouseButton", "RightMouseButton");
	AddTag(InputTagLeftMouseButton, "InputTag.LeftMouseButton", "LeftMouseButton");
	AddTag(InputTagAction1, "InputTag.Action1", "First action input");
	AddTag(InputTagAction2, "InputTag.Action2", "Second action input");
	AddTag(InputTagAction3, "InputTag.Action3", "Third action input");
	AddTag(InputTagAction4, "InputTag.Action4", "Forth action input");
	

	AddTag(MessageHealthPotion,  "Message.Health.Potion", "Message tag for Health Potion");
	AddTag(MessageHealthCrystal, "Message.Health.Crystal", "Message tag for Health Crystal");
	AddTag(MessageManaPotion,	"Message.Mana.Potion", "Message tag for Mana Potion");
	AddTag(MessageManaCrystal,	"Message.Mana.Crystal", "Message tag for Mana Crystal");
}

void FAuraGameplayTags::AddTag(FGameplayTag& GameplayTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	GameplayTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TagComment));
}



// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemGlobals.h"
#include "Core/AuraGameplayTags.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Interaction/AuraCombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/AuraPlayerController.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	InitMaxHealth(50.f);
	const FAuraGameplayTags auraGameplayTags = FAuraGameplayTags::Get();
	
	TagAttributeMap.Add(auraGameplayTags.AttributesPrimaryStrength, GetStrengthAttribute);
	TagAttributeMap.Add(auraGameplayTags.AttributesPrimaryIntelligence, GetIntelligenceAttribute);
	TagAttributeMap.Add(auraGameplayTags.AttributesPrimaryVigor, GetVigorAttribute);
	TagAttributeMap.Add(auraGameplayTags.AttributesPrimaryResiliance, GetResillianceAttribute);
	
	TagAttributeMap.Add(auraGameplayTags.AttributesSecondaryArmor, GetArmorAttribute);
	TagAttributeMap.Add(auraGameplayTags.AttributesSecondaryArmorPenetration, GetArmorPenetrationAttribute);
	TagAttributeMap.Add(auraGameplayTags.AttributesSecondaryCriticalHitChance, GetCriticalHitChanceAttribute);
	TagAttributeMap.Add(auraGameplayTags.AttributesSecondaryCriticalHitDamage, GetCriticalHitDamageAttribute);
	TagAttributeMap.Add(auraGameplayTags.AttributesSecondaryCriticalHitResistance, GetCriticalHitResistanceAttribute);
	TagAttributeMap.Add(auraGameplayTags.AttributesSecondaryBlockChance, GetBlockChanceAttribute);
	TagAttributeMap.Add(auraGameplayTags.AttributesSecondaryRegenHealth, GetHealthRegenAttribute);
	TagAttributeMap.Add(auraGameplayTags.AttributesSecondaryRegenMana, GetManaRegenAttribute);
	TagAttributeMap.Add(auraGameplayTags.AttributesSecondaryMaxHealth, GetMaxHealthAttribute);
	TagAttributeMap.Add(auraGameplayTags.AttributesSecondaryMaxMana, GetMaxManaAttribute);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/** Primary Attributes */
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilliance, COND_None, REPNOTIFY_Always);

	/** Secondary Attributes */
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);

	/** Vital Attributes */
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);

}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}

	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties properties;
	SetEffectProperties(Data, properties);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0, GetMaxHealth()));
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("%s Health: %d/%d"), *GetNameSafe(properties.TargetCharacter), static_cast<int>(GetHealth()), static_cast<int>(GetMaxHealth())), true, true, FLinearColor::Red, 4);
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0, GetMaxMana()));
	}

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float localIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);

		if (localIncomingDamage > 0)
		{
			const float newHealth = GetHealth() - localIncomingDamage;
			SetHealth(FMath::Clamp(newHealth, 0, GetMaxHealth()));
			const bool bIsFatal = newHealth <= 0;
			if (!bIsFatal)
			{
				FGameplayTagContainer tagContainer;
				tagContainer.AddTag(FAuraGameplayTags::Get().EffectHitReact);
				properties.TargetASC->TryActivateAbilitiesByTag(tagContainer);
			}
			else
			{
				IAuraCombatInterface* combatInterface = Cast<IAuraCombatInterface>(properties.TargetAvatarActor);
				if (combatInterface)
				{
					combatInterface->Die();
				}
			}

			const bool bBlockingHit = UAuraAbilitySystemLibrary::IsBlockingHit(properties.ContextHandle);
			const bool bCriticalHit = UAuraAbilitySystemLibrary::IsCriticalHit(properties.ContextHandle);
			ShowFloatingText(properties, localIncomingDamage, bCriticalHit, bBlockingHit);
		}		
	}
}

void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Strength, OldStrength);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Intelligence, OldIntelligence);
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Vigor, OldVigor);
}

void UAuraAttributeSet::OnRep_Resilliance(const FGameplayAttributeData& OldResilliance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Resilliance, OldResilliance);
}

void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Armor, OldArmor);
}

void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, ArmorPenetration, OldArmorPenetration);
}

void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, BlockChance, OldBlockChance);
}

void UAuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, CriticalHitChance, OldCriticalHitChance);
}

void UAuraAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, CriticalHitDamage, OldCriticalHitDamage);
}

void UAuraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, CriticalHitResistance, OldCriticalHitResistance);
}

void UAuraAttributeSet::OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, HealthRegen, OldHealthRegen);
}

void UAuraAttributeSet::OnRep_ManaRegen(const FGameplayAttributeData& OldManaRegen) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, ManaRegen, OldManaRegen);
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxMana, OldMaxMana);
}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data,
	FEffectProperties& EffectProperties)
{
	// Source = causer of effect
	// Target = target of effect (owner of this AS)
	EffectProperties.ContextHandle = Data.EffectSpec.GetContext();
	EffectProperties.SourceASC = EffectProperties.ContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	// Source
	if (IsValid(EffectProperties.SourceASC) && EffectProperties.SourceASC->AbilityActorInfo.IsValid() && EffectProperties.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		// Source Actor
		EffectProperties.SourceAvatarActor = EffectProperties.SourceASC->AbilityActorInfo->AvatarActor.Get();
		// Source Controller
		EffectProperties.SourceController = EffectProperties.SourceASC->AbilityActorInfo->PlayerController.Get(); 
		if (!EffectProperties.SourceController && EffectProperties.SourceAvatarActor)
		{
			// This means source object may not be player (It might be AI)
			if (const APawn* pawn = Cast<APawn>(EffectProperties.SourceAvatarActor))
			{
				EffectProperties.SourceController = pawn->GetController();
			}
		}
		if (EffectProperties.SourceController)
		{
			EffectProperties.SourceCharacter = Cast<ACharacter>(EffectProperties.SourceController->GetPawn());
		}
	}

	// Target
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		EffectProperties.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		EffectProperties.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		EffectProperties.TargetCharacter = Cast<ACharacter>(EffectProperties.TargetAvatarActor);
		EffectProperties.TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(EffectProperties.TargetAvatarActor);
	}
}

void UAuraAttributeSet::ShowFloatingText(const FEffectProperties& Properties, const float Damage, bool bIsCriticalDamage, bool bIsBlockingHit) const
{
	if (Properties.SourceCharacter != Properties.TargetCharacter)
	{
		AAuraPlayerController* pc = Cast<AAuraPlayerController>(Properties.SourceController);
		pc->ClientShowDamageText(Properties.TargetAvatarActor, Damage, bIsCriticalDamage, bIsBlockingHit);
	}
}
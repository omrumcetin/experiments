// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalcs/ExecCalcDamage.h"

#include "AbilitySystem/AuraAbilitySystemGlobals.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Core/AuraGameplayEffectTypes.h"
#include "Core/AuraGameplayTags.h"
#include "Interaction/AuraCombatInterface.h"

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	
	AuraDamageStatics()
	{
		// Target
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);

		// Source
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);
	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DStatics;
	return DStatics;
}

UExecCalcDamage::UExecCalcDamage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
}

void UExecCalcDamage::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* sourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* targetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	AActor* sourceAvatar = sourceASC->GetAvatarActor();
	AActor* targetAvatar = targetASC->GetAvatarActor();
	const IAuraCombatInterface* sourceCombatInterface = Cast<IAuraCombatInterface>(sourceAvatar);
	const IAuraCombatInterface* targetCombatInterface = Cast<IAuraCombatInterface>(targetAvatar);

	const UCharacterClassInfo* characterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(sourceAvatar);

	FGameplayEffectSpec owningSpec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* sourceTags = owningSpec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* targetTags = owningSpec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters aggParams;
	aggParams.SourceTags = sourceTags;
	aggParams.TargetTags = targetTags;

	float damage = owningSpec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().DataDamage);

	// -- data collection
	// target block chance
	float targetBlockChance = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, aggParams, targetBlockChance);
	targetBlockChance = FMath::Max<float>(targetBlockChance, 0.f);
	
	// target armor
	float targetArmor = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, aggParams, targetArmor);
	targetArmor = FMath::Max<float>(targetArmor, 0.f);

	// source penetration
	float sourceArmorPenetration = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, aggParams, sourceArmorPenetration);
	sourceArmorPenetration = FMath::Max<float>(sourceArmorPenetration, 0.f);

	// crit hit chance
	float sourceCritHitChance = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, aggParams, sourceCritHitChance);
	sourceCritHitChance = FMath::Max<float>(sourceCritHitChance, 0.f);

	float sourceCritHitDamage = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, aggParams, sourceCritHitDamage);
	sourceCritHitDamage = FMath::Max<float>(sourceCritHitDamage, 0.f);

	float targetCritResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, aggParams, targetCritResistance);
	targetCritResistance = FMath::Max<float>(targetCritResistance, 0.f);

	// get coefficients 
	FRealCurve* effectiveArmorCurve = characterClassInfo->DamageCoefficientTable->FindCurve(FName("EffectiveArmor"), FString());
	FRealCurve* armorPenetCurve = characterClassInfo->DamageCoefficientTable->FindCurve(FName("ArmorPenetration"), FString());

	// -- calculations --

	// coeffs
	const float coefArmorPenet = armorPenetCurve->Eval(sourceCombatInterface->GetPlayerLevel());
	const float coefEffectiveArmor = effectiveArmorCurve->Eval(targetCombatInterface->GetPlayerLevel());
	const bool bIsBlockingHit = FMath::FRand() * 100 < targetBlockChance;
	// if block halve the damage.
	damage = bIsBlockingHit ? damage / 2 : damage;
	const float effectiveArmor = targetArmor *= ( 100 - sourceArmorPenetration * coefArmorPenet ) / 100 ;
	damage *= (100 - effectiveArmor * coefEffectiveArmor ) / 100 ;
	// crit calculations
	//                                                          40           *=       (100 - 40 * 0.4) / 100 
	const bool bIsCriticalHit = FMath::FRand() * 100 < (sourceCritHitChance *= FMath::Abs((100 - sourceCritHitChance * targetCritResistance) / 100));
	if (bIsCriticalHit)
	{
		damage *= 4;
		damage += sourceCritHitDamage;
	}

	FGameplayEffectContextHandle effectContextHandle = owningSpec.GetContext();
	UAuraAbilitySystemLibrary::SetIsBlockingHit(effectContextHandle, bIsBlockingHit);
	UAuraAbilitySystemLibrary::SetIsCriticalHit(effectContextHandle, bIsCriticalHit);

	//FGameplayModifierEvaluatedData evaluatedData(DamageStatics().ArmorProperty, EGameplayModOp::Additive, armor);
	FGameplayModifierEvaluatedData evaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, damage);
	OutExecutionOutput.AddOutputModifier(evaluatedData);
}

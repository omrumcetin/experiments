// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMagCalcs/MMCMaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/AuraCombatInterface.h"

UMMCMaxHealth::UMMCMaxHealth()
{
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMCMaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather source from source and target
	const FGameplayTagContainer* sourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* targetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters evaluationParams;
	evaluationParams.SourceTags = sourceTags;
	evaluationParams.TargetTags = targetTags;

	float vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDef, Spec, evaluationParams, vigor);
	vigor = FMath::Max<float>(vigor, 0.f);

	IAuraCombatInterface* combatInterface = Cast<IAuraCombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 playerLevel = combatInterface->GetPlayerLevel();

	return 80.f + (2.5f * vigor) + 10.f * playerLevel;
}

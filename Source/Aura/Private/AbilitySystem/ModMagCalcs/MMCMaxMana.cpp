// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMagCalcs/MMCMaxMana.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/AuraCombatInterface.h"

UMMCMaxMana::UMMCMaxMana()
{
	IntelligenceDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false;
}

float UMMCMaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* sourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* targetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters evaluationParams;
	evaluationParams.SourceTags = sourceTags;
	evaluationParams.TargetTags = targetTags;

	float intelligence{1.f};
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, evaluationParams, intelligence);
	intelligence = FMath::Max<float>(intelligence, 0.f);

	IAuraCombatInterface* combatInterface = Cast<IAuraCombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 playerLevel = combatInterface->GetPlayerLevel();

	return 100.f + intelligence * 5.f + playerLevel * 10.f;
}

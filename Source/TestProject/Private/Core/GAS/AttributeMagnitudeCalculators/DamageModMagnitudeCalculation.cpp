// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GAS/AttributeMagnitudeCalculators/DamageModMagnitudeCalculation.h"

UDamageModMagnitudeCalculation::UDamageModMagnitudeCalculation()
{
	RelevantAttributesToCapture.Add(FDamageMagnitudeCapturedStatics::instance().HealthDef);
}

float UDamageModMagnitudeCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	float Result = 0.f;

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float DamageValue = Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Damage.Value")),
	                                                 true, -1.f);

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Health = 0.0f;
	GetCapturedAttributeMagnitude(FDamageMagnitudeCapturedStatics::instance().HealthDef, Spec, EvaluationParameters,
	                              Health);

	if (Health > 0)
	{
		Result = FMath::Clamp(DamageValue, 0, 1000);
	}

	return Result;
}

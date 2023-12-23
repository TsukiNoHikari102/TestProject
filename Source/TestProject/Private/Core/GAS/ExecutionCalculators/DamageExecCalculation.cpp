// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GAS/ExecutionCalculators/DamageExecCalculation.h"

#include "Core/GAS/GASAbilitySystemComponent.h"

struct FDamageExecutionCapturedStatics
{
	// Source
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	// Target
	DECLARE_ATTRIBUTE_CAPTUREDEF(IncomeDamageMult)

	FDamageExecutionCapturedStatics()
	{
		// Creating definitions for calculations

		// Source captures
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, Damage, Source, true);

		// Target captures
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, IncomeDamageMult, Target, false);
	}

	// singleton
	static FDamageExecutionCapturedStatics& instance()
	{
		static FDamageExecutionCapturedStatics INSTANCE;
		return INSTANCE;
	}
};

UDamageExecCalculation::UDamageExecCalculation()
{
	// Source
	RelevantAttributesToCapture.Add(FDamageExecutionCapturedStatics::instance().DamageDef);

	// Target
	RelevantAttributesToCapture.Add(FDamageExecutionCapturedStatics::instance().IncomeDamageMultDef);
}

void UDamageExecCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                    FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// Preparations

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Effect Context
	FGameplayEffectContextHandle Context = Spec.GetContext();

	// Ability that initiated this execution (can be nullptr!!)
	const UGASAbilitySystemComponent* GameplayAbility = Cast<UGASAbilitySystemComponent>(
		Context.GetAbilityInstance_NotReplicated());

	// Tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// Evaluate params setup
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;


	// Loading values from the structure with captured attributes

	float BaseDamage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(FDamageExecutionCapturedStatics::instance().DamageDef,
	                                                           EvaluateParameters, BaseDamage);

	float DamageIncomeMult = 1.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		FDamageExecutionCapturedStatics::instance().IncomeDamageMultDef, EvaluateParameters,
		DamageIncomeMult);


	// Culculating total damage.

	const float TotalDamage = BaseDamage * DamageIncomeMult;

	if (TotalDamage > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(UCharacterAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive,
			                               -TotalDamage));

		// If we do any damage, then any conditional GEs are allowed to fire (once we return)
		OutExecutionOutput.MarkConditionalGameplayEffectsToTrigger();
	}
}

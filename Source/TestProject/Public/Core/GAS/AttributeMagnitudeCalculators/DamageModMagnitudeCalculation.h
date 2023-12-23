// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "GameplayEffectExecutionCalculation.h"
#include "AbilitySystemComponent.h"
#include "Core/GAS/CharacterAttributeSet.h"
#include "DamageModMagnitudeCalculation.generated.h"

struct FDamageMagnitudeCapturedStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);

	FDamageMagnitudeCapturedStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, Health, Target, true);
	}

	// singleton
	static FDamageMagnitudeCapturedStatics& instance()
	{
		static FDamageMagnitudeCapturedStatics INSTANCE;
		return INSTANCE;
	}
};

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UDamageModMagnitudeCalculation : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UDamageModMagnitudeCalculation();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};

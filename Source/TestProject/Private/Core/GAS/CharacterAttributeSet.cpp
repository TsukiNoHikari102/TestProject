// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GAS/CharacterAttributeSet.h"

UCharacterAttributeSet::UCharacterAttributeSet()
{
	Health = 100.f;
	MaxHealth = 100.f;
	Stamina = 100.f;
	MaxStamina = 100.f;
	StaminaRegenRate = 2.f;
	Mana = 100.f;
	MaxMana = 100.f;
	Damage = 10.f;
	IncomeDamageMult = 1.f;
}

/**
 * Current attributes value change
 */
void UCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	ClampAttributeOnChange(Attribute, NewValue);
}

void UCharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

/**
 * Max attributes value change
 */
void UCharacterAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	ClampAttributeOnChange(Attribute, NewValue);
}

void UCharacterAttributeSet::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue,
                                                     float NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
}

void UCharacterAttributeSet::ClampAttributeOnChange(const FGameplayAttribute& AffectedAttribute, float& NewValue) const
{
	if (AffectedAttribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxHealth());
	}

	if (AffectedAttribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxHealth());
	}

	if (AffectedAttribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxStamina());
	}

	if (AffectedAttribute == GetMaxStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxStamina());
	}

	if (AffectedAttribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxMana());
	}

	if (AffectedAttribute == GetMaxManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxMana());
	}
}

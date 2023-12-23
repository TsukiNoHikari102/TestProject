// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UCharacterAttributeSet();

	// Health

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Health")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxHealth);

	// Stamina

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Stamina")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Stamina);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Stamina")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxStamina);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Stamina")
	FGameplayAttributeData StaminaRegenRate;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, StaminaRegenRate);

	// Mana
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Mana")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Mana);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Mana")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxMana);

	// Damage
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Damage")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Damage);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Damage")
	FGameplayAttributeData IncomeDamageMult;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, IncomeDamageMult);

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const override;

protected:
	virtual void ClampAttributeOnChange(const FGameplayAttribute& AffectedAttribute, float& NewValue) const;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Abilities/GameplayAbility.h"
#include "NativeGameplayTags.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Core/GAS/CoreGameplayAbility.h"
#include "PDA_CharacterAbilities.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UPDA_CharacterAbilities : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPDA_CharacterAbilities(){};
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gameplay")
	TArray<TSubclassOf<UCoreGameplayAbility>> DefaultAbilities;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gameplay")
	TArray<TSubclassOf<UGameplayEffect>> DefaultEffects;
};

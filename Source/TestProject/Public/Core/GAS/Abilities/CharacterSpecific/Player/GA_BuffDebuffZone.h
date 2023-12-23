// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/GAS/CoreGameplayAbility.h"
#include "Core/GAS/Abilities/CharacterSpecific/Common/AbilityActors/EffectZoneActor.h"
#include "GA_BuffDebuffZone.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UGA_BuffDebuffZone : public UCoreGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	TSubclassOf<AEffectZoneActor> EffectZoneClass;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
};

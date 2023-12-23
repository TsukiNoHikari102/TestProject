// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/GAS/CoreGameplayAbility.h"
#include "Core/GAS/GASCharacter.h"
#include "GA_Interact.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UGA_Interact : public UCoreGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Interact();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

private:
	UPROPERTY()
	AGASCharacter* AvatarActor;
};

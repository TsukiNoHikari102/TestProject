// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GAS/Abilities/CharacterSpecific/Player/GA_BouncingProjectile.h"

UGA_BouncingProjectile::UGA_BouncingProjectile()
{
}

void UGA_BouncingProjectile::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/GAS/CoreGameplayAbility.h"
#include "Weapon/Projectiles/Projectile.h"
#include "GA_BouncingProjectile.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UGA_BouncingProjectile : public UCoreGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_BouncingProjectile();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	TSubclassOf<AProjectile> ProjectileClass;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
};

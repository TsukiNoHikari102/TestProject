// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilPayload_Projectile.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TESTPROJECT_API UAbilPayload_Projectile : public UObject
{
	GENERATED_BODY()

public:
	UAbilPayload_Projectile();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn))
	FVector ImpactVelocity;
};

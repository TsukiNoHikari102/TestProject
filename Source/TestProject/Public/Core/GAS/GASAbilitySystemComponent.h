// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CoreGameplayAbility.h"
#include "GASAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class TESTPROJECT_API UGASAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Ability Management")
	void SetAbilityLevel(FGameplayAbilitySpecHandle AbilitySpecHandle,
	                     UCoreGameplayAbility* GameplayAbility, int32 Level);

	UFUNCTION(BlueprintCallable, Category = "Ability Management")
	bool SetGameplayEffectDurationHandle(FActiveGameplayEffectHandle Handle, float NewDuration);

private:
	FGameplayAbilitySpec* FindAbilitySpec(FGameplayAbilitySpecHandle Handle);
};

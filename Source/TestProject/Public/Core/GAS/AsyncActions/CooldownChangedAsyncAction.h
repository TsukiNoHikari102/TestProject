// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "CooldownChangedAsyncAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCooldownChangedPin, FGameplayTag, CooldownTag, float, TimeRemaining,
                                               float, Duration);

/**
 * 
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = AsyncTask))
class TESTPROJECT_API UCooldownChangedAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnCooldownChangedPin CooldownBegin;
	UPROPERTY(BlueprintAssignable)
	FOnCooldownChangedPin CooldownEnd;
	// Listens for changes (Begin and End) to cooldown GameplayEffects based on the cooldown tag.
	// UseServerCooldown determines if the Sever's cooldown is returned in addition to the local predicted cooldown.
	// If using ServerCooldown, TimeRemaining and Duration will return -1 to signal local predicted cooldown has begun.
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UCooldownChangedAsyncAction* ListenForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent,
	                                                            FGameplayTagContainer InCooldownTags,
	                                                            bool InUseServerCooldown);
	// You must call this function manually when you want the AsyncTask to end.
	// For UMG Widgets, you would call it in the Widget's Destruct event.
	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:
	UPROPERTY()
	UAbilitySystemComponent* ASC;
	FGameplayTagContainer CooldownTags;
	bool UseServerCooldown;
	// will be bound to OnActiveGameplayEffectAddedDelegateToSelf
	virtual void OnActiveGameplayEffectAddedCallback(UAbilitySystemComponent* Target,
	                                                 const FGameplayEffectSpec& SpecApplied,
	                                                 FActiveGameplayEffectHandle ActiveHandle);
	// Callback for RegisterGameplayTagEvent Added or Removed
	virtual void CooldownTagAddedRemoved(const FGameplayTag CooldownTag, int32 NewCount);
	// calculates cooldown for tag
	bool GetCooldownRemainingForTag(FGameplayTagContainer InCooldownTags, float& TimeRemaining,
	                                float& CooldownDuration);
};

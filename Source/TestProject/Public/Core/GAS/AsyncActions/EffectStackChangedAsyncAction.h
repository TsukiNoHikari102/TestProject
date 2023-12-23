// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AbilitySystemComponent.h"
#include "EffectStackChangedAsyncAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnGameplayEffectStackChangedPin, FGameplayTag, EffectGameplayTag,
                                              FActiveGameplayEffectHandle, Handle, int32, NewStackCount, int32,
                                              OldStackCount);

/**
 * 
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = AsyncTask))
class TESTPROJECT_API UEffectStackChangedAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintAssignable, Meta=(DisplayName = "EffectStackChange"))
	FOnGameplayEffectStackChangedPin OnGameplayEffectStackChange;
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UEffectStackChangedAsyncAction* ListenForGameplayEffectStackChange(
		UAbilitySystemComponent* AbilitySystemComponent, FGameplayTag InEffectGameplayTag);
	// You must call this function manually when you want the AsyncTask to end.
	// For UMG Widgets, you would call it in the Widget's Destruct event.
	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:
	UPROPERTY()
	UAbilitySystemComponent* ASC;
	FGameplayTag EffectGameplayTag;
	virtual void OnActiveGameplayEffectAddedCallback(UAbilitySystemComponent* Target,
	                                                 const FGameplayEffectSpec& SpecApplied,
	                                                 FActiveGameplayEffectHandle ActiveHandle);
	virtual void OnRemoveGameplayEffectCallback(const FActiveGameplayEffect& EffectRemoved);
	virtual void GameplayEffectStackChanged(FActiveGameplayEffectHandle EffectHandle, int32 NewStackCount,
	                                        int32 PreviousStackCount);
};

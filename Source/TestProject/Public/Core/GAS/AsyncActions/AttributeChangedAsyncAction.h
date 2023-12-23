// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AbilitySystemComponent.h"
#include "AttributeChangedAsyncAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChangedPin, FGameplayAttribute, Attribute, float, NewValue,
                                               float, OldValue);

/**
 * 
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = AsyncTask))
class TESTPROJECT_API UAttributeChangedAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedPin AttributeChanged; // this will create pin
	// Listens for an attribute changing.
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UAttributeChangedAsyncAction* ListenForAttributeChange(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAttribute Attribute);
	// Listens for an attribute changing.
	// Version that takes in an array of Attributes. Check the Attribute output for which Attribute changed.
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UAttributeChangedAsyncAction* ListenForAttributesChange(UAbilitySystemComponent* AbilitySystemComponent, TArray<FGameplayAttribute> Attributes);
	// You must call this function manually when you want the AsyncTask to end.
	// For UMG Widgets, you would call it in the Widget's Destruct event.
	UFUNCTION(BlueprintCallable)
	void EndTask();
	
protected:
	UPROPERTY()
	UAbilitySystemComponent* ASC;
	FGameplayAttribute AttributeToListenFor;
	TArray<FGameplayAttribute> AttributesToListenFor;
	// this will be bound to GetGameplayAttributeValueChangeDelegate
	void OnAttributeChanged(const FOnAttributeChangeData& Data);
};

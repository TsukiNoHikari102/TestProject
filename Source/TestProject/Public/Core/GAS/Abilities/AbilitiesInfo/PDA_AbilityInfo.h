// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/GAS/CoreGameplayAbility.h"
#include "Engine/DataAsset.h"
#include "PDA_AbilityInfo.generated.h"

USTRUCT(BlueprintType)
struct FAbilityUpgradeStructure
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TMap<FString, float> UpgradableParams;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayEffect>> AdditionalGameplayEffects;
};

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UPDA_AbilityInfo : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPDA_AbilityInfo();

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FText Name;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TSubclassOf<UCoreGameplayAbility> AbilityClass;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	UTexture2D* IconTexture;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FText Description;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	bool bCanBeUpgraded = false;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (EditCondition = "bCanBeUpgraded", EditConditionHides))
	TArray<FAbilityUpgradeStructure> AbilityLevelingInfo;
};

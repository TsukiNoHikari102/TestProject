// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Enums/AbilityInputEnum.h"
#include "CoreGameplayAbility.generated.h"

class UPDA_AbilityInfo;

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UCoreGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UCoreGameplayAbility();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
	EAbilityInput AbilitySlot = EAbilityInput::None;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
	TObjectPtr<UPDA_AbilityInfo> AbilityData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
	bool bRunOnAssign = false;
	
	UFUNCTION(BlueprintNativeEvent)
	void OnChangedLevel(int32 OldLevel, int32 NewLevel);
	void OnChangedLevel_Implementation(int32 OldLevel, int32 NewLevel);

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};

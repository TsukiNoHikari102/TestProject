// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GAS/CoreGameplayAbility.h"

#include "AbilitySystemComponent.h"

UCoreGameplayAbility::UCoreGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UCoreGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);
	if (bRunOnAssign == true)
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}

void UCoreGameplayAbility::OnChangedLevel_Implementation(int32 OldLevel, int32 NewLevel) {}

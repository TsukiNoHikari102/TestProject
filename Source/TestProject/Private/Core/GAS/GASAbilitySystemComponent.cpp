// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GAS/GASAbilitySystemComponent.h"

void UGASAbilitySystemComponent::SetAbilityLevel(FGameplayAbilitySpecHandle AbilitySpecHandle,
	UCoreGameplayAbility* GameplayAbility, int32 Level)
{
	auto Spec = FindAbilitySpec(AbilitySpecHandle);
	if (Spec)
	{
		const int32 OldLevel = Spec->Level;
		const int32 NewLevel = Level;
		Spec->Level = NewLevel;
	
		if (GameplayAbility)
		{
			GameplayAbility->OnChangedLevel(OldLevel, NewLevel);
		}
	}
}

bool UGASAbilitySystemComponent::SetGameplayEffectDurationHandle(FActiveGameplayEffectHandle Handle, float NewDuration)
{
	if (!Handle.IsValid())
	{
		return false;
	}

	const FActiveGameplayEffect* ActiveGameplayEffect = GetActiveGameplayEffect(Handle);
	if (!ActiveGameplayEffect)
	{
		return false;
	}

	FActiveGameplayEffect* AGE = const_cast<FActiveGameplayEffect*>(ActiveGameplayEffect);
	if (NewDuration > 0)
	{
		AGE->Spec.Duration = NewDuration;
	}
	else
	{
		AGE->Spec.Duration = 0.01f;
	}

	AGE->StartServerWorldTime = ActiveGameplayEffects.GetServerWorldTime();
	AGE->CachedStartServerWorldTime = AGE->StartServerWorldTime;
	AGE->StartWorldTime = ActiveGameplayEffects.GetWorldTime();
	//ActiveGameplayEffects.MarkItemDirty(*AGE);
	ActiveGameplayEffects.CheckDuration(Handle);

	AGE->EventSet.OnTimeChanged.Broadcast(AGE->Handle, AGE->StartWorldTime, AGE->GetDuration());
	OnGameplayEffectDurationChange(*AGE);

	return true;
}

FGameplayAbilitySpec* UGASAbilitySystemComponent::FindAbilitySpec(FGameplayAbilitySpecHandle Handle)
{
	FGameplayAbilitySpec* FoundAbility = nullptr;
	FoundAbility = FindAbilitySpecFromHandle(Handle);
	return FoundAbility;
}

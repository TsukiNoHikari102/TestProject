// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GAS/AsyncActions/CooldownChangedAsyncAction.h"

UCooldownChangedAsyncAction* UCooldownChangedAsyncAction::ListenForCooldownChange(
	UAbilitySystemComponent* AbilitySystemComponent, FGameplayTagContainer InCooldownTags, bool InUseServerCooldown)
{
	UCooldownChangedAsyncAction* ListenForCooldownChange = NewObject<UCooldownChangedAsyncAction>();
	ListenForCooldownChange->ASC = AbilitySystemComponent;
	ListenForCooldownChange->CooldownTags = InCooldownTags;
	ListenForCooldownChange->UseServerCooldown = InUseServerCooldown;
	if (!IsValid(AbilitySystemComponent) || InCooldownTags.Num() < 1)
	{
		ListenForCooldownChange->EndTask();
		return nullptr;
	}
	// bind OnActiveGameplayEffectAddedCallback
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(
		ListenForCooldownChange, &UCooldownChangedAsyncAction::OnActiveGameplayEffectAddedCallback);
	TArray<FGameplayTag> CooldownTagArray;
	InCooldownTags.GetGameplayTagArray(CooldownTagArray);
	for (FGameplayTag CooldownTag : CooldownTagArray)
	{
		// bind tag Added or Removed
		AbilitySystemComponent->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).AddUObject(
			ListenForCooldownChange, &UCooldownChangedAsyncAction::CooldownTagAddedRemoved);
	}
	return ListenForCooldownChange;
}

void UCooldownChangedAsyncAction::EndTask()
{
	if (IsValid(ASC))
	{
		ASC->OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll(this);
		TArray<FGameplayTag> CooldownTagArray;
		CooldownTags.GetGameplayTagArray(CooldownTagArray);
		for (FGameplayTag CooldownTag : CooldownTagArray)
		{
			ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
		}
	}
	SetReadyToDestroy();
	MarkAsGarbage();
}

void UCooldownChangedAsyncAction::OnActiveGameplayEffectAddedCallback(UAbilitySystemComponent* Target,
                                                                      const FGameplayEffectSpec& SpecApplied,
                                                                      FActiveGameplayEffectHandle ActiveHandle)
{
	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);
	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);
	TArray<FGameplayTag> CooldownTagArray;
	CooldownTags.GetGameplayTagArray(CooldownTagArray);
	for (FGameplayTag CooldownTag : CooldownTagArray)
	{
		if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
		{
			float TimeRemaining = 0.0f;
			float Duration = 0.0f;
			// Expecting cooldown tag to always be first tag
			FGameplayTagContainer CooldownTagContainer(GrantedTags.GetByIndex(0));
			GetCooldownRemainingForTag(CooldownTagContainer, TimeRemaining, Duration);
			if (ASC->GetOwnerRole() == ROLE_Authority)
			{
				// Player is Server
				CooldownBegin.Broadcast(CooldownTag, TimeRemaining, Duration);
			}
			else if (!UseServerCooldown && SpecApplied.GetContext().GetAbilityInstance_NotReplicated())
			{
				// Client using predicted cooldown
				CooldownBegin.Broadcast(CooldownTag, TimeRemaining, Duration);
			}
			else if (UseServerCooldown && SpecApplied.GetContext().GetAbilityInstance_NotReplicated() == nullptr)
			{
				// Client using Server's cooldown. This is Server's corrective cooldown GE.
				CooldownBegin.Broadcast(CooldownTag, TimeRemaining, Duration);
			}
			else if (UseServerCooldown && SpecApplied.GetContext().GetAbilityInstance_NotReplicated())
			{
				// Client using Server's cooldown but this is predicted cooldown GE.
				// This can be useful to gray out abilities until Server's cooldown comes in.
				CooldownBegin.Broadcast(CooldownTag, -1.0f, -1.0f);
			}
		}
	}
}

void UCooldownChangedAsyncAction::CooldownTagAddedRemoved(const FGameplayTag CooldownTag, int32 NewCount)
{
	if (NewCount == 0)
	{
		CooldownEnd.Broadcast(CooldownTag, -1.0f, -1.0f);
	}
}

bool UCooldownChangedAsyncAction::GetCooldownRemainingForTag(FGameplayTagContainer InCooldownTags, float& TimeRemaining,
                                                             float& CooldownDuration)
{
	if (IsValid(ASC) && InCooldownTags.Num() > 0)
	{
		TimeRemaining = 0.f;
		CooldownDuration = 0.f;
		FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(InCooldownTags);
		TArray<TPair<float, float>> DurationAndTimeRemaining = ASC->GetActiveEffectsTimeRemainingAndDuration(Query);
		if (DurationAndTimeRemaining.Num() > 0)
		{
			int32 BestIdx = 0;
			float LongestTime = DurationAndTimeRemaining[0].Key;
			for (int32 Idx = 1; Idx < DurationAndTimeRemaining.Num(); ++Idx)
			{
				if (DurationAndTimeRemaining[Idx].Key > LongestTime)
				{
					LongestTime = DurationAndTimeRemaining[Idx].Key;
					BestIdx = Idx;
				}
			}
			TimeRemaining = DurationAndTimeRemaining[BestIdx].Key;
			CooldownDuration = DurationAndTimeRemaining[BestIdx].Value;
			return true;
		}
	}
	return false;
}

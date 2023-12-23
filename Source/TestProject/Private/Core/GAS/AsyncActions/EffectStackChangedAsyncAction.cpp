// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GAS/AsyncActions/EffectStackChangedAsyncAction.h"

UEffectStackChangedAsyncAction* UEffectStackChangedAsyncAction::ListenForGameplayEffectStackChange(
	UAbilitySystemComponent* AbilitySystemComponent, FGameplayTag InEffectGameplayTag)
{
	UEffectStackChangedAsyncAction* ListenForGameplayEffectStackChange = NewObject<UEffectStackChangedAsyncAction>();
	ListenForGameplayEffectStackChange->ASC = AbilitySystemComponent;
	ListenForGameplayEffectStackChange->EffectGameplayTag = InEffectGameplayTag;
	if (!IsValid(AbilitySystemComponent) || !InEffectGameplayTag.IsValid())
	{
		ListenForGameplayEffectStackChange->EndTask();
		return nullptr;
	}
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(
		ListenForGameplayEffectStackChange, &UEffectStackChangedAsyncAction::OnActiveGameplayEffectAddedCallback);
	AbilitySystemComponent->OnAnyGameplayEffectRemovedDelegate().AddUObject(
		ListenForGameplayEffectStackChange, &UEffectStackChangedAsyncAction::OnRemoveGameplayEffectCallback);
	return ListenForGameplayEffectStackChange;
}

void UEffectStackChangedAsyncAction::EndTask()
{
	if (IsValid(ASC))
	{
		ASC->OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll(this);
		ASC->OnAnyGameplayEffectRemovedDelegate().RemoveAll(this);
	}
	SetReadyToDestroy();
	MarkAsGarbage();
}

void UEffectStackChangedAsyncAction::OnActiveGameplayEffectAddedCallback(UAbilitySystemComponent* Target,
                                                                         const FGameplayEffectSpec& SpecApplied,
                                                                         FActiveGameplayEffectHandle ActiveHandle)
{
	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);
	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);
	if (AssetTags.HasTagExact(EffectGameplayTag) || GrantedTags.HasTagExact(EffectGameplayTag))
	{
		ASC->OnGameplayEffectStackChangeDelegate(ActiveHandle)->AddUObject(
			this, &UEffectStackChangedAsyncAction::GameplayEffectStackChanged);
		OnGameplayEffectStackChange.Broadcast(EffectGameplayTag, ActiveHandle, 1, 0);
	}
}

void UEffectStackChangedAsyncAction::OnRemoveGameplayEffectCallback(const FActiveGameplayEffect& EffectRemoved)
{
	FGameplayTagContainer AssetTags;
	EffectRemoved.Spec.GetAllAssetTags(AssetTags);
	FGameplayTagContainer GrantedTags;
	EffectRemoved.Spec.GetAllGrantedTags(GrantedTags);
	if (AssetTags.HasTagExact(EffectGameplayTag) || GrantedTags.HasTagExact(EffectGameplayTag))
	{
		OnGameplayEffectStackChange.Broadcast(EffectGameplayTag, EffectRemoved.Handle, 0, 1);
	}
}

void UEffectStackChangedAsyncAction::GameplayEffectStackChanged(FActiveGameplayEffectHandle EffectHandle,
                                                                int32 NewStackCount, int32 PreviousStackCount)
{
	OnGameplayEffectStackChange.Broadcast(EffectGameplayTag, EffectHandle, NewStackCount, PreviousStackCount);
}

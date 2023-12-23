// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GAS/AsyncActions/AttributeChangedAsyncAction.h"

UAttributeChangedAsyncAction* UAttributeChangedAsyncAction::ListenForAttributeChange(
	UAbilitySystemComponent* AbilitySystemComponent, FGameplayAttribute Attribute)
{
	UAttributeChangedAsyncAction* WaitForAttributeChangedTask = NewObject<UAttributeChangedAsyncAction>();
	WaitForAttributeChangedTask->ASC = AbilitySystemComponent;
	WaitForAttributeChangedTask->AttributeToListenFor = Attribute;
	if (!IsValid(AbilitySystemComponent) || !Attribute.IsValid())
	{
		WaitForAttributeChangedTask->RemoveFromRoot();
		return nullptr;
	}
	// bind AttributeChanged
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(WaitForAttributeChangedTask, &UAttributeChangedAsyncAction::OnAttributeChanged);
	return WaitForAttributeChangedTask;
}

UAttributeChangedAsyncAction* UAttributeChangedAsyncAction::ListenForAttributesChange(
	UAbilitySystemComponent* AbilitySystemComponent, TArray<FGameplayAttribute> Attributes)
{
	UAttributeChangedAsyncAction* WaitForAttributeChangedTask = NewObject<UAttributeChangedAsyncAction>();
	WaitForAttributeChangedTask->ASC = AbilitySystemComponent;
	WaitForAttributeChangedTask->AttributesToListenFor = Attributes;
	if (!IsValid(AbilitySystemComponent) || Attributes.Num() < 1)
	{
		WaitForAttributeChangedTask->RemoveFromRoot();
		return nullptr;
	}
	// for each attribute
	for (FGameplayAttribute Attribute : Attributes)
	{
		// bind AttributeChanged
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(WaitForAttributeChangedTask, &UAttributeChangedAsyncAction::OnAttributeChanged);
	}
	return WaitForAttributeChangedTask;
}

void UAttributeChangedAsyncAction::EndTask()
{
	if (IsValid(ASC))
	{
		ASC->GetGameplayAttributeValueChangeDelegate(AttributeToListenFor).RemoveAll(this);
		for (FGameplayAttribute Attribute : AttributesToListenFor)
		{
			ASC->GetGameplayAttributeValueChangeDelegate(Attribute).RemoveAll(this);
		}
	}
	SetReadyToDestroy();
	MarkAsGarbage();
}

void UAttributeChangedAsyncAction::OnAttributeChanged(const FOnAttributeChangeData& Data)
{
	AttributeChanged.Broadcast(Data.Attribute, Data.NewValue, Data.OldValue);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GAS/Abilities/CharacterSpecific/Common/GA_Interact.h"

UGA_Interact::UGA_Interact()
{
	AbilitySlot = EAbilityInput::Interact;
}

void UGA_Interact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo,
                                   const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}
	else
	{
		AvatarActor = Cast<AGASCharacter>(GetAvatarActorFromActorInfo());
		const IInteractInterface* InteractiveActor = AvatarActor->GetInteractionComponent()->GetActiveInteractionObject();
		if (InteractiveActor == nullptr)
		{
			CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Interact"));
			InteractiveActor->Execute_Interact(InteractiveActor->_getUObject(), AvatarActor);
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		}
	}
}

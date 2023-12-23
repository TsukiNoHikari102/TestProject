// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GAS/Abilities/CharacterSpecific/Player/GA_BuffDebuffZone.h"

#include "Core/GAS/GASCharacter.h"
#include "Core/GAS/Abilities/AbilitiesInfo/PDA_AbilityInfo.h"

void UGA_BuffDebuffZone::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo,
                                         const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, false);
	}
	else
	{
		const auto Character = Cast<AGASCharacter>(GetAvatarActorFromActorInfo());
		const float CapsuleHalfHeight = Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		const FVector SpawnLocation{
			Character->GetActorLocation().X, Character->GetActorLocation().Y,
			Character->GetActorLocation().Z - CapsuleHalfHeight
		};
		GetWorld()->SpawnActor(EffectZoneClass, &SpawnLocation);
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
	}
}

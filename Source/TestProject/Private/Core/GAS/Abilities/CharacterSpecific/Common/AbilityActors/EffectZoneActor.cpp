// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GAS/Abilities/CharacterSpecific/Common/AbilityActors/EffectZoneActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Kismet/GameplayStatics.h"

AEffectZoneActor::AEffectZoneActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SetRootComponent(SphereCollision);
	SphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AEffectZoneActor::BeginPlay()
{
	Super::BeginPlay();
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AEffectZoneActor::OnCollisionBeginOverlap);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &AEffectZoneActor::OnCollisionEndOverlap);

	SetupActor();

	ApplyEffectsToChars();
	GetWorldTimerManager().SetTimer(ApplyEffectTimerH, this, &ThisClass::ApplyEffectsToChars, TriggerInterval, true);
}

void AEffectZoneActor::SetupActor()
{
	SphereCollision->SetSphereRadius(CollisionRadius);
	SetLifeSpan(LifeTime);
}

void AEffectZoneActor::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                               const FHitResult& SweepResult)
{
	const auto AbilityInterface = Cast<IAbilitySystemInterface>(OtherActor);
	const auto ASC = AbilityInterface->GetAbilitySystemComponent();
	if (ASC == nullptr) return;
	
	CharacterToEffectTriggers.Add(OtherActor, 0);
}

void AEffectZoneActor::OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const auto AbilityInterface = Cast<IAbilitySystemInterface>(OtherActor);
	const auto ASC = AbilityInterface->GetAbilitySystemComponent();
	if (ASC == nullptr) return;
	
	CharacterToEffectTriggers.Remove(OtherActor);
}

void AEffectZoneActor::ApplyEffectsToChars()
{
	for (auto& [Char, EffectTriggerCount] : CharacterToEffectTriggers)
	{
		// If actor is player
		if (Char == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
		{
			ApplyEffect(Char, PlayerHealEffect, HealAmount + EffectTriggerCount * HealIncrease);
		}
		else
		{
			// Apply effect to enemy
			ApplyEffect(Char, EnemiesDamageEffect, -DamageAmount - EffectTriggerCount * DamageIncrease);
		}
		EffectTriggerCount++;
	}
}

void AEffectZoneActor::ApplyEffect(AActor* Actor, TSubclassOf<UGameplayEffect> GameplayEffect, float Magnitude)
{
	auto AbilityInterface = Cast<IAbilitySystemInterface>(Actor);
	auto ASC = AbilityInterface->GetAbilitySystemComponent();
	if (!ASC) return;

	const FGameplayEffectContextHandle Context;
	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GameplayEffect, 0, Context);
	// Effect applying
	if (FGameplayEffectSpec* Spec = SpecHandle.Data.Get())
	{
		Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Ability.EffectZone.Value"), Magnitude);
		ASC->ApplyGameplayEffectSpecToSelf(*Spec);
	}
}

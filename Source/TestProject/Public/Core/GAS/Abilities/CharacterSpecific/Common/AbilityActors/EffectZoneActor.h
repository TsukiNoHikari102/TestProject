// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Characters/EnemyCharacter.h"
#include "Components/SphereComponent.h"
#include "Core/Player/PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "EffectZoneActor.generated.h"

UCLASS()
class TESTPROJECT_API AEffectZoneActor : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	USphereComponent* SphereCollision;

public:
	AEffectZoneActor();

protected:
	virtual void BeginPlay() override;

	virtual void SetupActor();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default", meta = (ExposeOnSpawn))
	float CollisionRadius = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default", meta = (ExposeOnSpawn))
	float LifeTime = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	TSubclassOf<UGameplayEffect> PlayerHealEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	float HealIncrease;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	TSubclassOf<UGameplayEffect> EnemiesDamageEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	float DamageIncrease;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default", meta = (ExposeOnSpawn))
	float TriggerInterval = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default", meta = (ExposeOnSpawn))
	float HealAmount = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default", meta = (ExposeOnSpawn))
	float DamageAmount = 2.f;

	UFUNCTION()
	void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                             const FHitResult& SweepResult);

	UFUNCTION()
	void OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                           int32 OtherBodyIndex);

private:
	void ApplyEffectsToChars();
	void ApplyEffect(AActor* Actor, TSubclassOf<UGameplayEffect> GameplayEffect, float Magnitude);

	TMap<TObjectPtr<AActor>, float> CharacterToEffectTriggers;

	FTimerHandle ApplyEffectTimerH;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectile.generated.h"

UCLASS()
class TESTPROJECT_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	AProjectile();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Movement")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Capsule Component")
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	float LifeSpanSeconds = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Damage")
	bool bCanDamageOwner = false;

	UFUNCTION(BlueprintNativeEvent)
	void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                           UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
	                           const FHitResult& SweepResult);
	virtual void OnCapsuleBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                                  UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
	                                                  bool bFromSweep,
	                                                  const FHitResult& SweepResult);
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Projectiles/Projectile.h"
#include "Core/GAS/GASCharacter.h"

AProjectile::AProjectile()
{
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetRootComponent(CapsuleComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement");
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnCapsuleBeginOverlap);

	SetLifeSpan(LifeSpanSeconds);
}

void AProjectile::OnCapsuleBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!GetWorld()) return;
	if (OtherActor == GetOwner() && bCanDamageOwner == false) return;

	const auto OwnerChar = Cast<AGASCharacter>(GetOwner());
	if (OwnerChar == nullptr) return;

	OwnerChar->OnMadeHitWithProjectileShot(this, OverlappedComponent, OtherActor, OtherComponent, OtherBodyIndex,
										 bFromSweep, SweepResult);

	const auto TargetChar = Cast<AGASCharacter>(OtherActor);
	if (TargetChar == nullptr)
	{
		Destroy();
	}
	
	TargetChar->ReceiveDamage(OwnerChar, SweepResult, OwnerChar->AttributeSet->GetDamage());
}

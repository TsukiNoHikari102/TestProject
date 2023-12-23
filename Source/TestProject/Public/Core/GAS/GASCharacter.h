// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
// CharacterMovement
#include "GameFramework/CharacterMovementComponent.h"
//Capsule component
#include "Components/CapsuleComponent.h"
// Enhanced Input
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
// Camera
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
// GAS
#include "GASAbilitySystemComponent.h"
#include "CharacterAttributeSet.h"
#include "AbilitySystemInterface.h"
#include "Data/PDA_CharacterAbilities.h"
#include "Enums/AbilityInputEnum.h"
// Interaction
#include "Components/InteractionComponent.h"
#include "Weapon/Projectiles/Projectile.h"
// UHT
#include "GASCharacter.generated.h"

UCLASS()
class TESTPROJECT_API AGASCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	UInteractionComponent* InteractionComponent;

public:
	AGASCharacter();

	/** GAS */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UGASAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UCharacterAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GAS)
	TObjectPtr<UPDA_CharacterAbilities> DefaultAbilitiesInfo;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void BeginPlay() override;

	virtual void SaveDefaults();

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = GAS)
	void ApplyAbilitiesData(TSoftObjectPtr<UPDA_CharacterAbilities> Data = nullptr, bool bOnlyEffects = false);

	UFUNCTION(BlueprintCallable)
	void ApplyRagdoll();

	UFUNCTION(BlueprintCallable)
	void ExitRagdoll();

public:
	/** Input */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void BindContext(TSoftObjectPtr<UInputMappingContext> Context = nullptr, APlayerController* OnController = nullptr);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BindASCInput(class UInputComponent* PlayerInputComponent = NULL);

	virtual void PossessedBy(AController* NewController) override;

	UInteractionComponent* GetInteractionComponent() const;

private:
	bool bGASInputBound = false;
	bool bDefaultAttributesApplied = false;

	virtual void SendAbilityLocalInput(const FInputActionValue& Value, int InputID);

	bool bRagdollEnabled = false;
	FTransform DefaultMeshTransform;
	ECollisionEnabled::Type DefaultMeshCollisionMode;
	ECollisionEnabled::Type DefaultCapsuleCollisionMode;

public:
	/** GAS Events */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnMadeHitWithProjectileShot(AProjectile* Projectile, UPrimitiveComponent* OverlappedComponent,
	                                 AActor* OtherActor,
	                                 UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
	                                 const FHitResult& SweepResult);
	virtual void OnMadeHitWithProjectileShot_Implementation(AProjectile* Projectile,
	                                                        UPrimitiveComponent* OverlappedComponent,
	                                                        AActor* OtherActor,
	                                                        UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
	                                                        bool bFromSweep,
	                                                        const FHitResult& SweepResult);

	// Not implemented yet
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnMadeHitWithHitscanShot(const FHitResult& Hit, float Damage);
	virtual void OnMadeHitWithHitscanShot_Implementation(const FHitResult& Hit, float Damage);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReceiveDamage(AGASCharacter* DamageInstigator, const FHitResult& Hit, float Damage);
	virtual void ReceiveDamage_Implementation(AGASCharacter* DamageInstigator, const FHitResult& Hit, float Damage);

	// Not implemented yet
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void MadeShotWithProjectile(AProjectile* Projectile);
	virtual void MadeShotWithProjectile_Implementation(AProjectile* Projectile);
};

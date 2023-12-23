// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GAS/GASCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Core/GAS/CoreGameplayAbility.h"
#include "Core/GAS/Abilities/AbilitiesInfo/PayloadObjects/AbilPayload_Projectile.h"

// Sets default values
AGASCharacter::AGASCharacter()
{
	// Attribute Set
	{
		AttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("Attribite Set"));
	}

	// Ability system component
	{
		AbilitySystemComponent = CreateDefaultSubobject<UGASAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	}

	// Camera boom
	{
		CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
		CameraBoom->SetRelativeLocation(FVector3d(0.0f, 0.0f, 8.492264f));
		CameraBoom->SetupAttachment(RootComponent);
		CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character
		CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
		CameraBoom->bInheritPitch = true;
		CameraBoom->bInheritYaw = true;
		CameraBoom->bInheritRoll = true;
	}

	// Follow Camera
	{
		FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
		FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
		// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
		FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
		FollowCamera->Deactivate(); // Deactivate camera by default to prevent performance issues
	}

	// Rotation
	{
		bUseControllerRotationPitch = false;
		bUseControllerRotationYaw = false;
		bUseControllerRotationRoll = false;
	}

	// Movement
	{
		UCharacterMovementComponent* CharMoveComp = GetCharacterMovement();
		CharMoveComp->bOrientRotationToMovement = true; // will force character to rotate towards velocity direction
		CharMoveComp->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // with rotation rate
		// Speed values
		CharMoveComp->JumpZVelocity = 700.f;
		CharMoveComp->AirControl = 0.35f;
		CharMoveComp->MaxWalkSpeed = 600.f;
		CharMoveComp->MinAnalogWalkSpeed = 20.f;
		CharMoveComp->BrakingDecelerationWalking = 2000.f;
	}

	// Capsule
	{
		UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
		CapsuleComp->InitCapsuleSize(35.0f, 90.0f);
	}

	// Mesh
	{
		USkeletalMeshComponent* MeshComp = GetMesh();
		if (MeshComp->GetSkeletalMeshAsset() == nullptr)
		{
			// set refrence here
			const FString MannequinAsset = TEXT(
				"/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'");
			const TSoftObjectPtr<USkeletalMesh> SKMesh(MannequinAsset);
			if (!SKMesh.IsNull())
			{
				MeshComp->SetSkeletalMesh(SKMesh.LoadSynchronous());
			}
			// Load ainm instance as well
			// since we loading blueprint class, add _C at the end of the asset refrence
			const FString AnimationInstanceAsset = TEXT(
				"/Script/Engine.AnimBlueprint'/Game/Characters/Mannequins/Animations/ABP_Quinn.ABP_Quinn_C'");
			const TSoftClassPtr<UAnimInstance> CAnimInst(AnimationInstanceAsset);
			if (!CAnimInst.IsNull())
			{
				MeshComp->SetAnimationMode(EAnimationMode::AnimationBlueprint);
				MeshComp->SetAnimInstanceClass(CAnimInst.LoadSynchronous());
			}
			// Apply default parameters to the mesh
			// Relative Location
			MeshComp->SetRelativeLocation(FVector3d(0.0f, 0.0f, -89.0f));
			// Relative Rotation
			MeshComp->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));
		}
	}

	// Interaction Component
	{
		InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
		InteractionComponent->SetupAttachment(GetMesh());
	}
}

// Called when the game starts or when spawned
void AGASCharacter::BeginPlay()
{
	Super::BeginPlay();

	BindContext(DefaultMappingContext);
	SaveDefaults();
}

void AGASCharacter::SaveDefaults()
{
	DefaultMeshTransform = GetMesh()->GetRelativeTransform();
	DefaultMeshCollisionMode = GetMesh()->GetCollisionEnabled();
	DefaultCapsuleCollisionMode = GetCapsuleComponent()->GetCollisionEnabled();
}

void AGASCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		// get forward vector
		FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		// get right vector
		FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AGASCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AGASCharacter::ApplyAbilitiesData(TSoftObjectPtr<UPDA_CharacterAbilities> Data, bool bOnlyEffects)
{
	if (AbilitySystemComponent == nullptr) return;

	const int Level = 1; // ability grant level
	if (bOnlyEffects == false)
	{
		for (TSubclassOf<UCoreGameplayAbility>& AbilityClass : Data->DefaultAbilities)
		{
			if (AbilityClass == nullptr) continue;
			// form ability info
			FGameplayAbilitySpec Spec{
				AbilityClass, Level,
				static_cast<int>(AbilityClass->GetDefaultObject<UCoreGameplayAbility>()->AbilitySlot),
				this
			};
			AbilitySystemComponent->GiveAbility(Spec);
		}
	}
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this); // owner of effect
	for (TSubclassOf<UGameplayEffect>& EffectClass : Data->DefaultEffects)
	{
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
			EffectClass, Level, EffectContext);
		if (SpecHandle.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void AGASCharacter::ApplyRagdoll()
{
	bRagdollEnabled = true;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetCharacterMovement()->StopMovementImmediately();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGASCharacter::ExitRagdoll()
{
	GetMesh()->SetCollisionEnabled(DefaultMeshCollisionMode);
	GetMesh()->SetAllBodiesSimulatePhysics(false);
	GetCapsuleComponent()->SetCollisionEnabled(DefaultCapsuleCollisionMode);

	GetMesh()->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	GetMesh()->SetRelativeTransform(DefaultMeshTransform);
}

void AGASCharacter::BindContext(TSoftObjectPtr<UInputMappingContext> Context, APlayerController* OnController)
{
	if (Context == nullptr)
	{
		Context = DefaultMappingContext;
	}
	if (Context == nullptr) return; // only because i don't want to crash app with check()
	APlayerController* PlayerController = OnController == nullptr
		                                      ? Cast<APlayerController>(GetController())
		                                      : OnController;
	if (PlayerController == nullptr)
	{
		return;
	}
	const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	// get enhanced input component's subsystem
	UEnhancedInputLocalPlayerSubsystem* EiSubsystem = ULocalPlayer::GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	ensure(LocalPlayer && EiSubsystem);
	// input component
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
	//EnhancedInputComponent->ClearActionBindings(); // clear all bindings if necessary
	EnhancedInputComponent->ClearBindingsForObject(this); // clear bindings only from this object
	if (EiSubsystem != nullptr)
	{
		// register mapping context
		EiSubsystem->AddMappingContext(Context.Get(), InputPriority);
	}
	// iterate over all mappings and put every unique action into the Map
	// since actions occur in the map multiple times, for every key binding
	TMap<FName, const UInputAction*> Actions;
	for (FEnhancedActionKeyMapping Mapping : Context->GetMappings())
	{
		FName ActionName = Mapping.Action->GetFName();
		if (!Actions.Contains(ActionName))
		{
			Actions.Add(ActionName, Mapping.Action.Get());
		}
		UE_LOG(LogTemp, Warning, TEXT("Action Name = %s"), *Mapping.Action->GetName())
	}
	// now differentiate actions in the loop and assign them according to the names
	// make sure to give action names accordingly
	for (const TPair<FName, const UInputAction*>& ActionInfo : Actions)
	{
		FString AName = ActionInfo.Key.ToString();
		const UInputAction* Action = ActionInfo.Value;
		// Confirm
		if (AName.Contains(TEXT("Confirm")) && AbilitySystemComponent != nullptr)
		{
			// bind Confirm action
			EnhancedInputComponent->BindAction(Action, ETriggerEvent::Started, AbilitySystemComponent.Get(),
			                                   &UAbilitySystemComponent::LocalInputConfirm);
			AbilitySystemComponent->GenericConfirmInputID = static_cast<int>(EAbilityInput::Confirm);
		}
		// Cancel
		if (AName.Contains(TEXT("Cancel")) && AbilitySystemComponent != nullptr)
		{
			// bind Confirm action
			EnhancedInputComponent->BindAction(Action, ETriggerEvent::Started, AbilitySystemComponent.Get(),
			                                   &UAbilitySystemComponent::LocalInputCancel);
			AbilitySystemComponent->GenericCancelInputID = static_cast<int>(EAbilityInput::Cancel);
		}
		// Jumping
		if (AName.Contains(TEXT("Jump")))
		{
			EnhancedInputComponent->BindAction(Action, ETriggerEvent::Triggered, this,
			                                   &AGASCharacter::SendAbilityLocalInput,
			                                   static_cast<int>(EAbilityInput::Jump));
			EnhancedInputComponent->BindAction(Action, ETriggerEvent::Completed, this,
			                                   &AGASCharacter::SendAbilityLocalInput,
			                                   static_cast<int>(EAbilityInput::Jump));
		}
		// Interact
		if (AName.Contains(TEXT("Interact")))
		{
			EnhancedInputComponent->BindAction(Action, ETriggerEvent::Started, this,
			                                   &AGASCharacter::SendAbilityLocalInput,
			                                   static_cast<int>(EAbilityInput::Interact));
		}
		// Skill1
		if (AName.Contains(TEXT("Skill1")))
		{
			EnhancedInputComponent->BindAction(Action, ETriggerEvent::Started, this,
			                                   &AGASCharacter::SendAbilityLocalInput,
			                                   static_cast<int>(EAbilityInput::Skill1));
		}
		// Skill2
		if (AName.Contains(TEXT("Skill2")))
		{
			EnhancedInputComponent->BindAction(Action, ETriggerEvent::Started, this,
			                                   &AGASCharacter::SendAbilityLocalInput,
			                                   static_cast<int>(EAbilityInput::Skill2));
		}
		// Dash
		if (AName.Contains(TEXT("Dash")))
		{
			EnhancedInputComponent->BindAction(Action, ETriggerEvent::Started, this,
			                                   &AGASCharacter::SendAbilityLocalInput,
			                                   static_cast<int>(EAbilityInput::Dash));
		}
		// Moving
		if (AName.Contains(TEXT("Move")))
		{
			EnhancedInputComponent->BindAction(Action, ETriggerEvent::Triggered, this, &AGASCharacter::Move);
		}
		// Looking around
		if (AName.Contains(TEXT("Look")))
		{
			EnhancedInputComponent->BindAction(Action, ETriggerEvent::Triggered, this, &AGASCharacter::Look);
		}
	}
}

void AGASCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	bool bIsPlayer = NewController->IsLocalPlayerController();
	// Enable Camera only when character posessed by player
	if (bIsPlayer)
	{
		FollowCamera->Activate();
	}
	if (AbilitySystemComponent != nullptr)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
	if (bDefaultAttributesApplied == false && DefaultAbilitiesInfo != nullptr)
	{
		ApplyAbilitiesData(DefaultAbilitiesInfo);
		bDefaultAttributesApplied = true;
	}
}

// Called to bind functionality to input
void AGASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	BindASCInput(PlayerInputComponent);
}

void AGASCharacter::BindASCInput(UInputComponent* PlayerInputComponent)
{
	if (bGASInputBound == true) return;

	UInputComponent* IC = PlayerInputComponent == nullptr ? InputComponent : PlayerInputComponent;
	if (AbilitySystemComponent == nullptr || IC == nullptr)
	{
		bGASInputBound = false;
		return;
	}

	FTopLevelAssetPath EnumAssetPath = {FName("/Script/TestProject"), FName("EAbilityInput")};
	AbilitySystemComponent->BindAbilityActivationToInputComponent(IC, FGameplayAbilityInputBinds{
		                                                              TEXT(""), TEXT(""), EnumAssetPath,
		                                                              static_cast<int>(EAbilityInput::Confirm),
		                                                              static_cast<int>(EAbilityInput::Cancel)
	                                                              });
	bGASInputBound = true;
}

UAbilitySystemComponent* AGASCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UInteractionComponent* AGASCharacter::GetInteractionComponent() const
{
	return InteractionComponent;
}

void AGASCharacter::SendAbilityLocalInput(const FInputActionValue& Value, int InputID)
{
	if (AbilitySystemComponent == nullptr) return;

	if (Value.Get<bool>() == true)
	{
		AbilitySystemComponent->AbilityLocalInputPressed(InputID);
	}
	else
	{
		AbilitySystemComponent->AbilityLocalInputReleased(InputID);
	}
}

void AGASCharacter::OnMadeHitWithProjectileShot_Implementation(AProjectile* Projectile,
                                                               UPrimitiveComponent* OverlappedComponent,
                                                               AActor* OtherActor, UPrimitiveComponent* OtherComponent,
                                                               int32 OtherBodyIndex, bool bFromSweep,
                                                               const FHitResult& SweepResult)
{
	TObjectPtr<UAbilPayload_Projectile> ProjectilePayload = NewObject<UAbilPayload_Projectile>(
		UAbilPayload_Projectile::StaticClass());
	ProjectilePayload->ImpactVelocity = Projectile->GetVelocity();
	

	{
		FGameplayEventData Payload;
		Payload.EventTag = FGameplayTag::RequestGameplayTag("Event.Weapon.HitObject");
		Payload.Instigator = this;
		Payload.Target = OtherActor;
		Payload.OptionalObject = ProjectilePayload;
		Payload.TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(SweepResult);
		
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			this, FGameplayTag::RequestGameplayTag("Event.Weapon.HitObject"), Payload);
	}

	auto ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
	if (ASC != nullptr)
	{
		FGameplayEventData Payload;
		Payload.EventTag = FGameplayTag::RequestGameplayTag("Event.Weapon.GotHit");
		Payload.Instigator = this;
		Payload.Target = OtherActor;
		Payload.OptionalObject = ProjectilePayload;
		Payload.TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(SweepResult);

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			OtherActor, FGameplayTag::RequestGameplayTag("Event.Weapon.GotHit"), Payload);
	}
}

void AGASCharacter::OnMadeHitWithHitscanShot_Implementation(const FHitResult& Hit, float Damage)
{
}

void AGASCharacter::ReceiveDamage_Implementation(AGASCharacter* DamageInstigator, const FHitResult& Hit, float Damage)
{
}

void AGASCharacter::MadeShotWithProjectile_Implementation(AProjectile* Projectile)
{
}

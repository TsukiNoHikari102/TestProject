// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/InteractableActor.h"

AInteractableActor::AInteractableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
}

void AInteractableActor::Interact_Implementation(AActor* InteractionInstigator)
{
	IInteractInterface::Interact_Implementation(InteractionInstigator);

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Interact"));

	Destroy();
}

void AInteractableActor::SetInteractionEnabled_Implementation(bool bCanInteract)
{
	IInteractInterface::SetInteractionEnabled_Implementation(bCanInteract);
}

bool AInteractableActor::GetInteractionEnabled_Implementation()
{
	return IInteractInterface::GetInteractionEnabled_Implementation();
}

void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
}



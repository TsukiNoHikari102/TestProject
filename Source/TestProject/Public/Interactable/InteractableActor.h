// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractInterface.h"
#include "InteractableActor.generated.h"

UCLASS()
class TESTPROJECT_API AInteractableActor : public AActor, public IInteractInterface
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;
	
public:	
	AInteractableActor();
	
	virtual void Interact_Implementation(AActor* InteractionInstigator) override;
	virtual void SetInteractionEnabled_Implementation(bool bCanInteract) override;
	virtual bool GetInteractionEnabled_Implementation() override;

protected:
	virtual void BeginPlay() override;

	bool bCanBeInteracted = true;
};

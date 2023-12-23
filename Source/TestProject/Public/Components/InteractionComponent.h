// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "Interfaces/InteractInterface.h"
#include "InteractionComponent.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UInteractionComponent : public UCapsuleComponent
{
	GENERATED_BODY()

public:
	UInteractionComponent();
	IInteractInterface* GetActiveInteractionObject() const;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnInteractableBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                const FHitResult& SweepResult);

	UFUNCTION()
	void OnInteractableEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	IInteractInterface* ActiveInteractionObject;
};

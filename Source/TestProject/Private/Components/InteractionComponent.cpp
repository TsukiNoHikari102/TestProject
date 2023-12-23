// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractionComponent.h"

UInteractionComponent::UInteractionComponent() {}

IInteractInterface* UInteractionComponent::GetActiveInteractionObject() const
{
	return ActiveInteractionObject;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnInteractableBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnInteractableEndOverlap);
}

void UInteractionComponent::OnInteractableBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass())) return;
	UE_LOG(LogTemp, Display, TEXT("Component %s Begin Overlap"), *OtherActor->GetName());

	ActiveInteractionObject = Cast<IInteractInterface>(OtherActor);
}

void UInteractionComponent::OnInteractableEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass())) return;
	UE_LOG(LogTemp, Display, TEXT("Component %s End Overlap"), *OtherActor->GetName());
	if (ActiveInteractionObject != Cast<IInteractInterface>(OtherActor)) return;

	if (!OtherActor || !Cast<AActor>(ActiveInteractionObject) || !ActiveInteractionObject) return;
	ActiveInteractionObject = nullptr;
}
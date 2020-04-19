// Copyright 2020. All rights reserved. A prototype by CodingRook.


#include "InteractionComponent.h"
#include "ChronoGameInstance.h"
#include "Engine/World.h"
#include "ChronoCharacter.h"
#include "InteractablePropBase.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInteractionComponent::ExecuteInteraction(AChronoCharacter* Instigator)
{
	// As a minimum send notify that this interaction interacted on its owning prop actor
	if (AInteractablePropBase* OwnerProp = Cast<AInteractablePropBase>(GetOwner()))
	{
		OwnerProp->OnWasInteractedWith();
	}
}

FText UInteractionComponent::GetInteractionText()
{
	return InteractionText;
}

FText UInteractionComponent::GetAlternativeInteractionText()
{
	return AlternativeInteractionText;
}

// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

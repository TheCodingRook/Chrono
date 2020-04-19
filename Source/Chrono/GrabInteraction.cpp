// Copyright 2020. All rights reserved. A prototype by CodingRook.


#include "GrabInteraction.h"
#include "ChronoCharacter.h"
#include "InteractablePropBase.h"
#include "GrabbingAbility.h"

UGrabInteraction::UGrabInteraction()
{
	InteractionText = FText::FromString("Grab").ToUpper();
	AlternativeInteractionText = FText::FromString("Swap to").ToUpper();
}

void UGrabInteraction::ExecuteInteraction(AChronoCharacter* Instigator)
{
	Super::ExecuteInteraction(Instigator);
	
	AInteractablePropBase* PropToGrab = Cast<AInteractablePropBase>(GetOwner());

	/** Get the GrabbingAbility component via the array returned by GetComponents() */
	TInlineComponentArray<UGrabbingAbility*> GrabComponents;
	Instigator->GetComponents(GrabComponents);
	
	// We expect this to be the first and only component of its kind!
	GrabComponents.Top()->GrabObject(PropToGrab);
	PropToGrab->SetIsInteractedWith(true);
	
}
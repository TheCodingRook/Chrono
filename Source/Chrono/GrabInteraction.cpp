// Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.


#include "GrabInteraction.h"
#include "ChronoCharacter.h"
#include "InteractablePropBase.h"
#include "GrabbingAbility.h"

void UGrabInteraction::ExecuteInteraction(AActor* Instigator)
{
	Super::ExecuteInteraction(Instigator);
	
	AInteractablePropBase* PropToGrab = Cast<AInteractablePropBase>(GetOwner());

	// Recast the instigating actor to AChronoCharacter
	if (AChronoCharacter * InteractingCharacter = Cast<AChronoCharacter>(Instigator))
	{
		/** Get the GrabbingAbility component via the array returned by GetComponents() */
		TInlineComponentArray<UGrabbingAbility*> GrabComponents;
		InteractingCharacter->GetComponents(GrabComponents);
	
		// We expect this to be the first and only component of its kind!
		GrabComponents.Top()->GrabObject(PropToGrab);
		PropToGrab->SetIsInteractedWith(true);
		//InteractingCharacter->SetGrabButtonDown(true);
	}
}
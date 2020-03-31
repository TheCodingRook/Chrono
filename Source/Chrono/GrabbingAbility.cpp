// Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.


#include "GrabbingAbility.h"
#include "ChronoCharacter.h"
#include "Templates\Casts.h"
#include "Components\ArrowComponent.h" // TODO: Can probably remove: only for intellisense
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "InteractablePropBase.h"

float UGrabbingAbility::GetGrabDistance() const
{
	return GrabDistance;
}

void UGrabbingAbility::GrabObject(AActor* In_AActor)
{
	OwnerCharacter = CastChecked<ACharacter>(GetOwner());

	if (AInteractablePropBase* PropToGrab = Cast<AInteractablePropBase>(In_AActor))
	{
		// Check first to see that we are not already grabbing something 
		if (bIsAlreadyGrabbingSomething)
		{
			// Are we holding the same thing we are trying to grab?
				if (PropToGrab == PropAlreadyHeld)
				{
					return; // If so, do nothing and return;
				}

				else
				{
					// We are holding something different so drop that and will pick up new prop/object
					DropObject();
				}
		}

		/** Pick up the prop, whether by physicshandle or attaching to a socket */
		
		if (PropToGrab->ActorHasTag(AttachableTag))
		{
			// This prop can be attached to a socket, but first stop simulating physics!
			PropToGrab->GetMesh()->SetSimulatePhysics(false);
			PropToGrab->GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
			PropToGrab->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "GrabSocket");
		}

		else
		{
			// We can alternativel pick this prop up using the default physics handle component
			PropToGrab->GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
			GrabComponentAtLocation(PropToGrab->GetMesh(), NAME_None, PropToGrab->GetMesh()->GetCenterOfMass());
		}

		// Notify listeners that the grabbing ability's owner has interacted with a prop/object (used for widget purposes)
		OnPropInteraction.Broadcast();
		
		// We are now already holding something so set the necessary member fields
		bIsAlreadyGrabbingSomething = true;
		PropAlreadyHeld = PropToGrab;
	}
}

void UGrabbingAbility::DropObject()
{
	//First check if we are using the physics handle; if so the pointer GrabbedComponent will be non-null
	if (GrabbedComponent) {
		// Reset the collision channel for pawn back to block
		GrabbedComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		ReleaseComponent();
	}

	// Otherwise let's see if we have a prop that we are grabbing/holding (the PropAlreadyHeld is non-null)
	else if (PropAlreadyHeld)
	{
		// Detach from socket, and re-enable physics
		PropAlreadyHeld->GetRootComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		PropAlreadyHeld->GetMesh()->SetSimulatePhysics(true);
		PropAlreadyHeld->GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	}

	else
	{
		// We are not holding anything, either via the physicshandle or a socket. Do nothing and return
		return;
	}

	// Notify listeners that the grabbing ability's owner has stoped interacting with a prop/object (used for widget purposes)
	OnEndedPropInteraction.Broadcast();

	/** Reset the necessary member fields*/
	PropAlreadyHeld->SetIsInteractedWith(false);
	bIsAlreadyGrabbingSomething = false;
	PropAlreadyHeld = nullptr;
}

FName UGrabbingAbility::GetAttachableTag()
{
	return AttachableTag;
}

void UGrabbingAbility::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GrabbedComponent)
	{
		// If this component/actor was grabbed without being attached to a specific socket (i.e. it wasn't "attachable")..
		if (!GrabbedComponent->GetOwner()->ActorHasTag(AttachableTag))
		{
			// ... update its location every tick to be in fron of ChronoCharacter
			SetTargetLocation(OwnerCharacter->ActorToWorld().GetLocation() + OwnerCharacter->GetActorForwardVector() * GrabDistance);
		}
	}
}

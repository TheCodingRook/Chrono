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

void UGrabbingAbility::GrabObject()
{
	OwnerCharacter = CastChecked<ACharacter>(GetOwner());

	// Check first to see if there is a prop we can grab 
	if (AvailablePropToGrab != nullptr)
	{
		if (AvailablePropToGrab->ActorHasTag(AttachableTag)) 
		{
			// This prop can be attached to a socket, but first stop simulating physics!
			AvailablePropToGrab->GetMesh()->SetSimulatePhysics(false);
			AvailablePropToGrab->GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
			AvailablePropToGrab->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "GrabSocket");
		}
		else
		{
			// We can alternativel pick this prop up using the default physics handle component
			AvailablePropToGrab->GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
			GrabComponentAtLocation(AvailablePropToGrab->GetMesh(), NAME_None, AvailablePropToGrab->GetMesh()->GetCenterOfMass());
		}

		// Notify listeners that the grabbing ability's owner has interacted with a prop/object (used for widget purposes)
		OnPropInteraction.Broadcast();
	}

	else
	{
		// We have nothing to grab - do noting and return.
		return;
	}
}

void UGrabbingAbility::DropObject()
{
	// First make sure we are indeed grabbing something! First check that we are using the physics handle; if so the pointer GrabbedComponent will be non-null
	if (GrabbedComponent) {
		// Reset the collision channel for pawn back to block
		GrabbedComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		ReleaseComponent();
	}

	// Otherwise let's see if we have a prop that we are grabbing/holding (the AvailablePropToGrab is non-null)
	else if (AvailablePropToGrab)
	{
		// Detach from socket, and re-enable physics
		AvailablePropToGrab->GetRootComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		AvailablePropToGrab->GetMesh()->SetSimulatePhysics(true);
		AvailablePropToGrab->GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	}

	else
	{
		// Do nothing and return
		return;
	}

	// Remember to put the pointer back to null!
	AvailablePropToGrab = nullptr;

	// Notify listeners that the grabbing ability's owner has stoped interacting with a prop/object (used for widget purposes)
	OnEndedPropInteraction.Broadcast();
}

FName UGrabbingAbility::GetAttachableTag()
{
	return AttachableTag;
}

void UGrabbingAbility::SetAvailblePropToGrab(AInteractablePropBase* InPropToGrab)
{
	if (AvailablePropToGrab == nullptr)
	{
		AvailablePropToGrab = InPropToGrab;
	}
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

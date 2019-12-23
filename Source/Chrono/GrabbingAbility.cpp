// Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.


#include "GrabbingAbility.h"
#include "ChronoCharacter.h"
#include "Templates\Casts.h"
#include "Components\ArrowComponent.h" // TODO: Can probably remove: only for intellisense
#include "DrawDebugHelpers.h"

float UGrabbingAbility::GetGrabDistance() const
{
	return GrabDistance;
}

void UGrabbingAbility::GrabObject()
{
	OwnerCharacter = CastChecked<ACharacter>(GetOwner());

	FHitResult OutHitResult;

	// Add a bit of a Z-axis offset to the start vector (we want the grab sweep and debug lines to start from chest area of Character)
	// TODO Vaggelis: Remember to amend this when changing skeletal mesh eventually, or consider using a socket location, i.e. clavicle
	FVector StartVector = OwnerCharacter->ActorToWorld().GetLocation() + FVector(0.f, 0.f, 30.f);
	FVector EndVector = StartVector + (OwnerCharacter->GetActorForwardVector() * GrabDistance);
	
	FCollisionQueryParams GrabQueryParameters;
	GrabQueryParameters.AddIgnoredActor(OwnerCharacter);
	
	/* Commented-out debug lines for future use if necessary */
	// DrawDebugLine(GetWorld(), StartVector, EndVector, FColor::Red, false , 1.f,(uint8)'\000', GrabRadius);
	// DrawDebugSphere(GetWorld(), EndVector, GrabRadius, 16, FColor::Green, false, 1, 10.f);

	bool FoundSomethingToGrab = GetWorld()->SweepSingleByChannel(
		OutHitResult,
		StartVector,
		EndVector,
		FQuat::Identity,
		ECC_PhysicsBody,
		FCollisionShape::MakeSphere(GrabRadius),
		GrabQueryParameters
	);

	// If there is anything within reach (GrabDistance)
	if (FoundSomethingToGrab)
	{
		// Disable collision for now so we can carry the object without crashing into it
		OutHitResult.GetComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		// Grab from centre of mass so that it is easier to handle.
		// TODO Vaggelis: How do I remove rotation from grabbed object?
		GrabComponentAtLocation(OutHitResult.GetComponent(), NAME_None, OutHitResult.GetComponent()->GetCenterOfMass());
		
	}
}

void UGrabbingAbility::DropObject()
{
	// First make sure we are indeed grabbing something!
	if (GrabbedComponent) {
		// Reset the collision channel for pawn back to block
		GrabbedComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		ReleaseComponent();
	}
}

void UGrabbingAbility::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GrabbedComponent)
	{
		SetTargetLocation(OwnerCharacter->ActorToWorld().GetLocation() + OwnerCharacter->GetActorForwardVector() * GrabDistance);
	}
}

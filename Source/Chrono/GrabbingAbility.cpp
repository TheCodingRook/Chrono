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

	FVector StartVector = OwnerCharacter->ActorToWorld().GetLocation();
	FVector EndVector = StartVector + (OwnerCharacter->GetActorForwardVector() * GrabDistance);
	
	UE_LOG(LogTemp, Warning, TEXT("EndVector is: %s"), *EndVector.ToString())
	

	FCollisionQueryParams GrabQueryParameters;
	GrabQueryParameters.AddIgnoredActor(OwnerCharacter);
	DrawDebugLine(GetWorld(), StartVector, StartVector + (OwnerCharacter->GetActorForwardVector() * GrabDistance), FColor::Red, false , 1.f,(uint8)'\000', 10.f);

	bool FoundSomethingToGrab = GetWorld()->LineTraceSingleByChannel(
		OutHitResult,
		StartVector,
		EndVector,
		ECollisionChannel::ECC_PhysicsBody,
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

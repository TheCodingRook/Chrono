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
	

	FCollisionQueryParams GrabParameters;
	GrabParameters.AddIgnoredActor(OwnerCharacter);
	DrawDebugLine(GetWorld(), StartVector, StartVector + (OwnerCharacter->GetActorForwardVector() * GrabDistance), FColor::Red, false , 1.f,(uint8)'\000', 10.f);

	bool FoundSomethingToGrab = GetWorld()->LineTraceSingleByChannel(
		OutHitResult,
		StartVector,
		EndVector,
		ECollisionChannel::ECC_PhysicsBody,
		GrabParameters
	);

	// If there is anything within reach (GrabDistance)
	if (FoundSomethingToGrab)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reached out to grab: %s"), *OutHitResult.GetComponent()->GetName())
		GrabComponentAtLocation(OutHitResult.GetComponent(), NAME_None, EndVector);
		
	}
}

void UGrabbingAbility::DropObject()
{
	ReleaseComponent();
}

void UGrabbingAbility::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GrabbedComponent)
	{
		SetTargetLocation(OwnerCharacter->ActorToWorld().GetLocation() + OwnerCharacter->GetActorForwardVector() * GrabDistance);
	}
}

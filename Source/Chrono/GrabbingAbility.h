// Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "GrabbingAbility.generated.h"

/**
 * Custom PhysicsHandleComponent to implement grabbing functionality
 */
UCLASS()
class CHRONO_API UGrabbingAbility : public UPhysicsHandleComponent
{
	GENERATED_BODY()

public:
	/* Getter methods */

	// Getter for GrabDistance value
	UFUNCTION(BlueprintPure, Category = "Grab configuration")
	float GetGrabDistance() const;

protected:
	// Minimum distance before character can grab an object
	UPROPERTY(EditAnywhere, Category = "Grab configuration")
	float GrabDistance;
	
};

// Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "GrabbingAbility.generated.h"

/**
 * Custom PhysicsHandleComponent to implement grabbing functionality
 */
UCLASS(collapsecategories, ClassGroup = Physics, hidecategories = Object, meta = (BlueprintSpawnableComponent))
class CHRONO_API UGrabbingAbility : public UPhysicsHandleComponent
{
	GENERATED_BODY()

public:
	/* Getter methods */

	// Getter for GrabDistance value
	UFUNCTION(BlueprintPure, Category = "Grab configuration")
	float GetGrabDistance() const;

	UFUNCTION(BlueprintCallable, Category = "Grab configuration")
	void GrabObject();

	UFUNCTION(BlueprintCallable, Category = "Grab configuration")
	void DropObject();

protected:
	// Minimum distance before character can grab an object
	// NOTE: in centimeters!
	UPROPERTY(EditAnywhere, Category = "Grab configuration")
	float GrabDistance = 100.f;

	// Radius of grabbing range when facing forward
	UPROPERTY(EditAnywhere, Category = "Grab configuration")
	float GrabRadius = 0.25f;

	// Called every frame
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
private:
	// Private member to store the pointer to the owning Character (cast will be necessary depending on which
	// characters this ability is attached to
	class ACharacter* OwnerCharacter;
};

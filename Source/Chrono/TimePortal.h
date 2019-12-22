// Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimePortal.generated.h"

/*
 *Simple base class to implement time portals in the game. Main responsibilities include
 *sending out an event dispatch to inform of a time-travel event, and resetting scene or world
 *by moving objects or other actors to thei original location (or potential location of a specific
 * point in time dictated by player)
 */
UCLASS()
class CHRONO_API ATimePortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimePortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	class UBoxComponent* CollisionBox;

};

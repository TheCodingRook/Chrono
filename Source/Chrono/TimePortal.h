// Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimePortal.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPortalTraversal);

/*
  Simple base class to implement time portals in the game. Main responsibilities include
  sending out an event dispatch to inform of a time-travel event, and resetting scene or world
  by moving objects or other actors to thei original location (or potential location of a specific
  point in time dictated by player)
 */
UCLASS()
class CHRONO_API ATimePortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimePortal();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Delegate to notify that character (or anyone else) has traversed this Time Portal to anyone who's listening to this event
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Event Dispatchers", meta = (AllowPrivateAccess = "true"))
	FOnPortalTraversal OnPortalTraversal;

protected:
	// Boolean to toggle the portal
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Portal", meta = (AllowPrivateAccess = "true"))
	bool bIsActive = true;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Set up components for the time portal in order to implement both the in-game functionality but also
	// some additional elements to help setup and placement while working in the Editor
	//
	//
	// Default root SceneComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time Portal", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DefaultRoot;

	// Bounds for the portal that triggers time travel
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time Portal", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* PortalTrigger;

	// Bounds for the trigger that allows through-access on the flip-side of the portal
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time Portal", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* AllowThroughTrigger;

	// Mesh to render the portal projector on
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time Portal", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PortalScreen;

	// The teleport point that this portal is linked to (in space not in time; this is a location on the level)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time Portal", meta = (AllowPrivateAccess = "true"))
	USceneComponent* TeleportLocation;

	// A "helper" text render so we can see exactly where the teleport spawn point is while working in the Editor
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time Portal", meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* TeleportLocationText;

	// An additional "helper" in the form of an arrow so we can see exactly where the teleport spawn point is while working in the Editor
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time Portal", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* TeleportLocationArrow;

	// SceneCapture component that will display where the character will spawn to in space (and perhaps specific point in time!)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time Portal", meta = (AllowPrivateAccess = "true"))
	class USceneCaptureComponent2D* PortalProjector;
	//
	//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

};

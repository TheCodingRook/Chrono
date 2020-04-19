// Copyright 2020. All rights reserved. A prototype by CodingRook.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TimeTravelComponent.h" // Have to include this here because I cannot forward declare the FTimestampedActions struct in the ReplayPastActions method declaration below
#include "PastSelfController.generated.h"

/**
 * Custom APlayerController-derived class to possess a "past self" ChronoCharacter 
 */
UCLASS()
class CHRONO_API APastSelfController : public APlayerController
{
	GENERATED_BODY()

public:
	/* Getter for the reference of this controller's character*/
	UFUNCTION(BlueprintPure, Category = "Character")
	class AChronoCharacter* GetMyChronoCharacter() const;

	UFUNCTION(BlueprintCallable, Category = "Time Travel", meta = (AllowPrivateAccess = "true"))
	void ReplayPastActions(const FTimestampedActions& ActionsToReplay);

protected:
	virtual void OnPossess(APawn* ControlledPawn) override;
	
private:
	// Store pointer to this controller's character
	AChronoCharacter* MyChronoCharacter;
};

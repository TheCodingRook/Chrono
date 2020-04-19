// Copyright 2020. All rights reserved. A prototype by CodingRook.

#pragma once

#include "CoreMinimal.h"
#include "InteractionComponent.h"
#include "GrabInteraction.generated.h"

/**
 * Component class to specifically implement picking up / grabbing of props in the world when the interact button is pressed
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CHRONO_API UGrabInteraction : public UInteractionComponent
{
	GENERATED_BODY()
	
protected:
	
	UGrabInteraction();

	virtual void ExecuteInteraction(AChronoCharacter* Instigator) override;
};

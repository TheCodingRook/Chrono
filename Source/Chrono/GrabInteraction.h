// Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.

#pragma once

#include "CoreMinimal.h"
#include "InteractionComponent.h"
#include "GrabInteraction.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CHRONO_API UGrabInteraction : public UInteractionComponent
{
	GENERATED_BODY()
	
protected:
	virtual void ExecuteInteraction(AActor* Instigator) override;
};

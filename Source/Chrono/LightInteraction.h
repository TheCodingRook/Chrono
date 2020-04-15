// Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.

#pragma once

#include "CoreMinimal.h"
#include "InteractionComponent.h"
#include "LightInteraction.generated.h"

/**
 * Simple base class for the Light Interaction command (to light torches, braziers and fires in general)
 */
UCLASS(ClassGroup = (Custom), Blueprintable, meta = (BlueprintSpawnableComponent))
class CHRONO_API ULightInteraction : public UInteractionComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interaction")
	void LightFire(AChronoCharacter* Instigator);

protected:
	ULightInteraction();
	virtual void ExecuteInteraction(AChronoCharacter* Instigator) override;
};

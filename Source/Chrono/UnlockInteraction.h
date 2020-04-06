// Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.

#pragma once

#include "CoreMinimal.h"
#include "InteractionComponent.h"
#include "UnlockInteraction.generated.h"


UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class CHRONO_API UUnlockInteraction : public UInteractionComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interaction")
	void UnlockProp(AActor* Instigator);

protected:
	virtual void ExecuteInteraction(AActor* Instigator) override;
		
};

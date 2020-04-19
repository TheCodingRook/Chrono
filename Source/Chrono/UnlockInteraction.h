// Copyright 2020. All rights reserved. A prototype by CodingRook.

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
	void UnlockProp(AChronoCharacter* Instigator);

protected:
	UUnlockInteraction();
	virtual void ExecuteInteraction(AChronoCharacter* Instigator) override;
		
};

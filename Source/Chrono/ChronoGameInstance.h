// Copyright 2020. All rights reserved. A prototype by CodingRook.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ChronoGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CHRONO_API UChronoGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Interaction Commands")
	void PushNewInteractionCommand(UInteractionComponent* NewInteraction);

	UFUNCTION(BlueprintCallable, Category = "Interaction Commands")
	void PopInteractionCommand(UInteractionComponent* InteractionCommandToRemove);

	UFUNCTION(BlueprintCallable, Category = "Interaction Commands")
	UInteractionComponent* GetLatestInteractionCommand();

	UFUNCTION(BlueprintPure, Category = "Interaction Commands")
	TArray<UInteractionComponent*> GetInteractionCommandStack() { return InteractionCommandStack; }

private:
	UInteractionComponent* CurrentInteractionCommand;
	TArray<UInteractionComponent*> InteractionCommandStack;
	
};

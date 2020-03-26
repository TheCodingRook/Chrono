// Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.

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
	class UInteractionComponent* GetCurrentInteractionCommand();

	UFUNCTION(BlueprintCallable, Category = "Interaction Commands")
	void RegisterCurrentInteractionCommand(UInteractionComponent* InCommand);

	UFUNCTION(BlueprintCallable, Category = "Interaction Commands")
	void DeRegisterCurrentInteractionCommand();

private:
	UInteractionComponent* CurrentInteractionCommand;
	
};

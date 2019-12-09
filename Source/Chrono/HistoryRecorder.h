// A prototype by Evangelos ("Vaggelis") Tsesmelidakis. All rights reserved. 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "HistoryRecorder.generated.h"

/**
 * 
 */
UCLASS()
class CHRONO_API UHistoryRecorder : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "Basic")
	FString SaveSlotName;

	UHistoryRecorder();
	
};

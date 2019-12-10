// A prototype by Evangelos ("Vaggelis") Tsesmelidakis. All rights reserved. 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "HistoryRecorder.generated.h"

USTRUCT(BlueprintType)
struct FRecordedHistory
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Recording")
		TArray<float> InputControlInfo;
};

/**
 * Customized class to record gameplay and simulate a "travel back in time" operation
 */
UCLASS()
class CHRONO_API UHistoryRecorder : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "Basic")
	FString SaveSlotName;

	UHistoryRecorder();

	UPROPERTY(VisibleAnywhere, Category = "Recording")
	FRecordedHistory RecordedHistory;
	
};

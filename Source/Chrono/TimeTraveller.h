// A prototype by Evangelos ("Vaggelis") Tsesmelidakis. All rights reserved. 2019

#pragma once

#include "CoreMinimal.h"
#include "ChronoCharacter.h"
#include "TimeTraveller.generated.h"

// Forward declerations
class UHistoryRecorder;

/**
 * 
 */
UCLASS()
class CHRONO_API ATimeTraveller : public AChronoCharacter
{
	GENERATED_BODY()

	ATimeTraveller();

public:
	UFUNCTION(BlueprintPure, Category = "Time Travel")
	UHistoryRecorder* GetTimeTravelComponent() const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Time Travel")
	UHistoryRecorder* TimeTravelComponent;
	
};

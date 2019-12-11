// A prototype by Evangelos ("Vaggelis") Tsesmelidakis. All rights reserved. 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimeTravelComponent.generated.h"

/*
*	Simple struct to store a specific input action and the exact time it was received
*/
USTRUCT(BlueprintType)
struct FRecordedInputAction
{
	GENERATED_USTRUCT_BODY()

	float TimeStamp;
	FName ActionName;
};


/*	A class that allows the actor (in this case most likely the controlled pawn) to
* 	store its controller's input so it can be re-played at a later time. Intended to
*	be used to simulate a Player travelling back in time and encountering "past self".
*/	
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHRONO_API UTimeTravelComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTimeTravelComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	/* Public getter functions */
	UFUNCTION(BlueprintPure, Category = "Time Travel")
	TArray<FRecordedInputAction> GetRecordedInputActions() const;

	UFUNCTION(BlueprintPure, Category = "Time Travel")
	float GetMaxRecordingTime() const;

	UFUNCTION(BlueprintPure, Category = "Time Travel")
	int GetMaxStructArraySize() const;

	UFUNCTION(BlueprintPure, Category = "Time Travel")
	bool ShouldRecord() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
private:
	// Array of structs to record a continuous stream of input actions
	UPROPERTY(EditDefaultsOnly, Category = "Time Travel")
	TArray<FRecordedInputAction> PastActions;

	// Maximum "history recording" time
	UPROPERTY(EditDefaultsOnly, Category = "Time Travel")
	float MaxRecordingTime; // in seconds
	
	// Maximum size that the struct of recorded input actions can grow to
	UPROPERTY(EditDefaultsOnly, Category = "Time Travel")
	int MaxStructArraySize;

	UPROPERTY(EditDefaultsOnly, Category = "Time Travel")
	bool bShouldRecord;
};

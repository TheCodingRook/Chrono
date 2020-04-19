// Copyright 2020. All rights reserved. A prototype by CodingRook.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimeTravelComponent.generated.h"

//	Utility to be able to print the enumeration types easily for debugging purposes.
template <typename TEnum>
static FORCEINLINE FString EnumToString(const FString& Name, TEnum Value)
{
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
	if (!enumPtr)
	{
		return FString("Invalid Enum!");
	}
	return enumPtr->GetDisplayNameTextByIndex((int32)Value).ToString();
};

// Struct representing a recorded action and its associated value
USTRUCT(BlueprintType)
struct FRecordedActionInput
{
	GENERATED_USTRUCT_BODY()

	// Default constructor
	FRecordedActionInput();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Struct Contents")
	FName RecordedAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Struct Contents")
		float InputValue;

};

// Struct to store unique timestamp entries with associated list of recorded actions
USTRUCT(BlueprintType)
struct FTimestampedActions
{
	GENERATED_USTRUCT_BODY()

	// Default constructor
	FTimestampedActions();

	// Constructor with a known timestamp
	FTimestampedActions(float ts);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Struct Contents")
	float TimeStamp = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Struct Contents")
	TArray<FRecordedActionInput> RecordedActionInputArray;
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
	TArray<FTimestampedActions> GetTimestampedInputsArray() const;

	UFUNCTION(BlueprintPure, Category = "Time Travel")
	float GetMaxRecordingTime() const;

	UFUNCTION(BlueprintPure, Category = "Time Travel")
	int GetMaxStructArraySize() const;

	UFUNCTION(BlueprintPure, Category = "Time Travel")
	bool ShouldRecord() const;


	//Add to the array of timestamped inputs
	UFUNCTION(BlueprintCallable, Category = "Time Travel")
	void AddTimestampedInput(float TimeStamp, FRecordedActionInput NewRecordedActionInput);
	
	// Reset all the movement and action arrays
	UFUNCTION(BlueprintCallable, Category = "Time Travel")
	void WipeHistory();

	UFUNCTION(BlueprintCallable, Category = "Time Travel")
	void AllowRecording(bool bInCanRecord);

	/*	This is the struct object that will hold the number of different input values depending on how many
	movement and action bindings we create. It will be the basis for replicating all the elements of the
	struct array that will hold input values for every timestamp of recording. This way we can dynamically
	code as many or as few recordable actions we want without explicitly coding the description of each of
	those in a specific enum or other struct. Once we know how many actions/movements the final game will
	have we can switch to describing each one of them explicitly.
 */
	FTimestampedActions TimestampedInputsTemplate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	


	// Array of structs where the timestamp entry is unique and all input values for that entry are recorded
	UPROPERTY(VisibleAnywhere, Category = "Time Travel")
	TArray<FTimestampedActions> TimestampedInputsArray;

	// Maximum "history recording" time
	UPROPERTY(EditDefaultsOnly, Category = "Time Travel")
	float MaxRecordingTime; // in seconds

	// Maximum size that the struct of recorded input actions can grow to
	UPROPERTY(EditDefaultsOnly, Category = "Time Travel")
	int MaxStructArraySize;

	// Flat to determine whether input actions should be recorded
	UPROPERTY(EditDefaultsOnly, Category = "Time Travel")
	bool bShouldRecord;

	// Keeps track of whether the character is a past version of itself or not
	UPROPERTY(EditDefaultsOnly, Category = "Time Travel")
	bool bIsPastSelf;
};
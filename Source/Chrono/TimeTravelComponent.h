// A prototype by Evangelos ("Vaggelis") Tsesmelidakis. All rights reserved. 2019

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

// Struct to store unique timestamp entries with associated values for all input actions in that timestamp
USTRUCT(BlueprintType)
struct FTimestampedInputs
{
	GENERATED_USTRUCT_BODY()

	// Default constructor
	FTimestampedInputs();

	// Constructor with a known timestamp
	FTimestampedInputs(float ts);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Struct Contents")
	float TimeStamp = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Struct Contents")
	TArray<float> InputValues;
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
	TArray<FTimestampedInputs> GetTimestampedInputsArray() const;

	UFUNCTION(BlueprintPure, Category = "Time Travel")
	float GetMaxRecordingTime() const;

	UFUNCTION(BlueprintPure, Category = "Time Travel")
	int GetMaxStructArraySize() const;

	UFUNCTION(BlueprintPure, Category = "Time Travel")
	bool ShouldRecord() const;


	//Add to the array of timestamped inputs
	UFUNCTION(BlueprintCallable, Category = "Time Travel")
	void AddTimestampedInput(float TimeStamp, int32 InputValuesArrayIndex, float RecordedValue);
	
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
	FTimestampedInputs TimestampedInputsTemplate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	


	// Array of structs where the timestamp entry is unique and all input values for that entry are recorded
	UPROPERTY(VisibleAnywhere, Category = "Time Travel")
	TArray<FTimestampedInputs> TimestampedInputsArray;

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
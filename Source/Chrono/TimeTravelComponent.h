// A prototype by Evangelos ("Vaggelis") Tsesmelidakis. All rights reserved. 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimeTravelComponent.generated.h"

/*Enumeration to store name of actions */
UENUM(BlueprintType)
enum class EInputActionEnum : uint8
{
	Move_Forward UMETA(DisplayName = "Move Forward"),
	Move_Right UMETA(DisplayName = "Move Right"),
	Turn UMETA(DisplayName = "Turn"),
	Turn_At_Rate UMETA(DisplayName = "Turn At Rate"),
	Look_Up UMETA(DisplayName = "Look Up"),
	Look_Up_At_Rate UMETA(DisplayName = "Look Up At Rate"),
	Jump UMETA(DisplayName = "Jump"),
	Stop_Jumping UMETA(DisplayName = "Stop Jumping"),
	Fire UMETA(DisplayName = "Fire")
};

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

	// Constructor with known initializing values
	FTimestampedInputs(float ts, float jv, float sjv, float mfv, float mrv, float tv, float tarv, float luv, float luarv, float fv);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Struct Contents")
	float TimeStamp = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Struct Contents")
		float JumpValue = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Struct Contents")
		float StopJumpingValue = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Struct Contents")
		float MoveForwardValue = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Struct Contents")
		float MoveRightValue = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Struct Contents")
		float TurnValue = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Struct Contents")
		float TurnAtRateValue = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Struct Contents")
		float LookUpValue = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Struct Contents")
		float LookUpAtRateValue = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Struct Contents")
		float FireValue = 0;
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
	TArray<FTimestampedInputs> GetTimestampedInputs() const;

	UFUNCTION(BlueprintPure, Category = "Time Travel")
	float GetMaxRecordingTime() const;

	UFUNCTION(BlueprintPure, Category = "Time Travel")
	int GetMaxStructArraySize() const;

	UFUNCTION(BlueprintPure, Category = "Time Travel")
	bool ShouldRecord() const;


	//Add to the array of timestamped inputs
	UFUNCTION(BlueprintCallable, Category = "Time Travel")
	void AddTimestampedInput(float TimeStamp, EInputActionEnum RecordedActionName, float RecordedValue);
	
	// Reset all the movement and action arrays
	UFUNCTION(BlueprintCallable, Category = "Time Travel")
	void WipeHistory();

	UFUNCTION(BlueprintCallable, Category = "Time Travel")
	void AllowRecording(bool bInCanRecord);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	// Array of structs where the timestamp entry is unique and all input values for that entry are recorded
	UPROPERTY(VisibleAnywhere, Category = "Time Travel")
	TArray<FTimestampedInputs> TimestampedInputs;

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

/** Helper functions */

// Helper to update the input float values in a TimestampedInputs entry
void AmendTimestampedInputEntry(FTimestampedInputs& Out_TimestampedInputs, EInputActionEnum RecordedActionName, float RecordedValue);
// A prototype by Evangelos ("Vaggelis") Tsesmelidakis. All rights reserved. 2019


#include "TimeTravelComponent.h"


// Default constructor - no implementation
// TODO Vaggelis: decide if I truly need this constructor
FTimestampedInputs::FTimestampedInputs()
{}

// Constructor with only a known timestamp
FTimestampedInputs::FTimestampedInputs(float ts)
	:TimeStamp{ ts }
{}



// Sets default values for this component's properties
UTimeTravelComponent::UTimeTravelComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Set member variables to default values
	MaxRecordingTime = 600.0f; // TODO: Default is 10 minutes; is this enough?
	MaxStructArraySize = 3600; // TODO: At 60 frames per second that is 10 minutes; Is this enough? Should it be "frame-rate safe"?
	bShouldRecord = true;	// Initial setting so recording can begin the first time

}

// Called when the game starts
void UTimeTravelComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UTimeTravelComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TArray<FTimestampedInputs> UTimeTravelComponent::GetTimestampedInputsArray() const
{
	return TimestampedInputsArray;
}

float UTimeTravelComponent::GetMaxRecordingTime() const
{
	return MaxRecordingTime;
}

int UTimeTravelComponent::GetMaxStructArraySize() const
{
	return MaxStructArraySize;
}

bool UTimeTravelComponent::ShouldRecord() const
{
	return bShouldRecord;
}

void UTimeTravelComponent::AddTimestampedInput(float RecordedTimeStamp, int32 InputValuesArrayIndex, float RecordedValue)
{
	// If this is a non-empty array
	if (TimestampedInputsArray.Num() > 0)
	{
		// Check the last entry's timestamp
		if (TimestampedInputsArray.Top().TimeStamp == RecordedTimeStamp)
		{
			UE_LOG(LogTemp, Warning, TEXT("Instructed to record with value: %f"), RecordedValue)
			// If same then amend the float variable in the InputValues array at the index passed in
			TimestampedInputsArray.Top().InputValues[InputValuesArrayIndex] = RecordedValue;

			// End the method here by returning
			return;
		}
	}

	// Otherwise add a new element (whether this is the very first entry in the array,or if we are 
	// dealing with a new timestamp)
	// In order to add a new element (and more importantly the very first one) we need a copy of the 
	// struct template first, set up its values (timestamp and float value in the correct index within
	// the InputValues array, and then add it to the TArray of TimestampedInputs
	UE_LOG(LogTemp, Warning, TEXT("Instructed to record with value: %f"), RecordedValue)
	FTimestampedInputs NewEntry = TimestampedInputsTemplate;
	NewEntry.TimeStamp = RecordedTimeStamp;
	NewEntry.InputValues[InputValuesArrayIndex] = RecordedValue;
	UE_LOG(LogTemp, Warning, TEXT("Inserting value: %f"), RecordedValue)
	TimestampedInputsArray.Add(NewEntry);
		
	
}

void UTimeTravelComponent::WipeHistory()
{
	// Empty timestamp array so as not compound historic actions
	TimestampedInputsArray.Empty();
}

void UTimeTravelComponent::AllowRecording(bool bInCanRecord)
{
	bShouldRecord = bInCanRecord;
}


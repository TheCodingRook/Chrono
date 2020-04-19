// Copyright 2020. All rights reserved. A prototype by CodingRook.


#include "TimeTravelComponent.h"

// Default constructor for FRecordedActionInput creating an empty/null recording
FRecordedActionInput::FRecordedActionInput()
{
	RecordedAction = NAME_None;
	InputValue = 0.f;
}

// Default constructor - no implementation
// TODO Vaggelis: decide if I truly need this constructor
FTimestampedActions::FTimestampedActions()
{}

// Constructor with only a known timestamp
FTimestampedActions::FTimestampedActions(float ts)
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

TArray<FTimestampedActions> UTimeTravelComponent::GetTimestampedInputsArray() const
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

void UTimeTravelComponent::AddTimestampedInput(float RecordedTimeStamp, FRecordedActionInput NewRecordedActionInput) // TODO Vaggelis: Should improve on 
{
	// If this is a non-empty array
	if (TimestampedInputsArray.Num() > 0)
	{
		// Check the last entry's timestamp
		if (TimestampedInputsArray.Top().TimeStamp == RecordedTimeStamp)
		{
			// If same then amend then just add a new recorded action for same timestamp
			TimestampedInputsArray.Top().RecordedActionInputArray.Add(NewRecordedActionInput);
			
			// End the method here by returning
			return;
		}
	}

	// Otherwise add a new element (whether this is the very first entry in the array,or if we are 
	// dealing with a new timestamp)
	// In order to add a new element (and more importantly the very first one) we need a copy of the 
	// struct template first, set up its values (timestamp and float value in the correct index within
	// the InputValues array, and then add it to the TArray of TimestampedInputs
	FTimestampedActions NewEntry;
	NewEntry.TimeStamp = RecordedTimeStamp;
	NewEntry.RecordedActionInputArray.Add(NewRecordedActionInput);
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


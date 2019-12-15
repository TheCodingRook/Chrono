// A prototype by Evangelos ("Vaggelis") Tsesmelidakis. All rights reserved. 2019


#include "TimeTravelComponent.h"

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

// Called every frame
void UTimeTravelComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TArray<FRecordedInputAction> UTimeTravelComponent::GetPastActions() const
{
	return PastActions;
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

void UTimeTravelComponent::AddRecordedAction(float RecordedTimeStamp, EInputActionEnum RecordedActionName, float RecordedValue)
{
	// Createa  new struct object with the inputs
	FRecordedInputAction NewAction;
	NewAction.TimeStamp = RecordedTimeStamp;
	NewAction.ActionName = RecordedActionName;
	NewAction.Value = RecordedValue;
	// Add to array
	PastActions.Add(NewAction);
}

void UTimeTravelComponent::AllowRecording(bool bInCanRecord)
{
	bShouldRecord = bInCanRecord;
}

// Called when the game starts
void UTimeTravelComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}





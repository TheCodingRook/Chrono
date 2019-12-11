// A prototype by Evangelos ("Vaggelis") Tsesmelidakis. All rights reserved. 2019


#include "HistoryRecorder.h"

// Sets default values for this component's properties
UHistoryRecorder::UHistoryRecorder()
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
void UHistoryRecorder::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TArray<FRecordedInputAction> UHistoryRecorder::GetRecordedInputActions() const
{
	return PastActions;
}

float UHistoryRecorder::GetMaxRecordingTime() const
{
	return MaxRecordingTime;
}

int UHistoryRecorder::GetMaxStructArraySize() const
{
	return MaxStructArraySize;
}

bool UHistoryRecorder::ShouldRecord() const
{
	return bShouldRecord;
}

// Called when the game starts
void UHistoryRecorder::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}





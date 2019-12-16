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

TArray<FRecordedInputAction> UTimeTravelComponent::GetPastActions() const
{
	return PastActions;
}

TArray<FUniqueTimeStamp> UTimeTravelComponent::GetUniqueTimeStamps() const
{
	return UniqueTimeStamps;
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

void UTimeTravelComponent::AddUniqueTimeStamp(float RecordedTimeStamp, EInputActionEnum ActionToAdd, float RecordedValue)
{
	// Createa  new struct object with the float inputs
	FUniqueTimeStamp NewUniqueTimeStamp;
	NewUniqueTimeStamp.TimeStamp = RecordedTimeStamp;

	switch (ActionToAdd)
	{
	case EInputActionEnum::Jump:
		NewUniqueTimeStamp.JumpValue = RecordedValue;
		break;

	case EInputActionEnum::Stop_Jumping:
		NewUniqueTimeStamp.StopJumpingValue = RecordedValue;
		break;

	case EInputActionEnum::Move_Forward:
		NewUniqueTimeStamp.MoveForwardValue = RecordedValue;
		break;

	case EInputActionEnum::Move_Right:
		NewUniqueTimeStamp.MoveRightValue = RecordedValue;
		break;

	case EInputActionEnum::Turn:
		NewUniqueTimeStamp.TurnValue = RecordedValue;
		break;

	case EInputActionEnum::Turn_At_Rate:
		NewUniqueTimeStamp.TurnAtRateValue = RecordedValue;
		break;

	case EInputActionEnum::Look_Up:
		NewUniqueTimeStamp.LookupValue = RecordedValue;
		break;

	case EInputActionEnum::Look_Up_At_Rate:
		NewUniqueTimeStamp.LookUpAtRateValue = RecordedValue;
		break;

	case EInputActionEnum::Fire:
		NewUniqueTimeStamp.FireValue = RecordedValue;
		break;

	default:
		break;
	}
	// Add to array
	UniqueTimeStamps.Add(NewUniqueTimeStamp);
}

void UTimeTravelComponent::AddDuplicateTimeStamp(EInputActionEnum RecordedActionName, float RecordedValue)
{
	switch (RecordedActionName)
	{
	case EInputActionEnum::Jump:
		UniqueTimeStamps.Top().JumpValue = RecordedValue;
		break;

	case EInputActionEnum::Stop_Jumping:
		UniqueTimeStamps.Top().StopJumpingValue = RecordedValue;
		break;

	case EInputActionEnum::Move_Forward:
		UniqueTimeStamps.Top().MoveForwardValue = RecordedValue;
		break;

	case EInputActionEnum::Move_Right:
		UniqueTimeStamps.Top().MoveRightValue = RecordedValue;
		break;

	case EInputActionEnum::Turn:
		UniqueTimeStamps.Top().TurnValue = RecordedValue;
		break;

	case EInputActionEnum::Turn_At_Rate:
		UniqueTimeStamps.Top().TurnAtRateValue = RecordedValue;
		break;

	case EInputActionEnum::Look_Up:
		UniqueTimeStamps.Top().LookupValue = RecordedValue;
		break;

	case EInputActionEnum::Look_Up_At_Rate:
		UniqueTimeStamps.Top().LookUpAtRateValue = RecordedValue;
		break;

	case EInputActionEnum::Fire:
		UniqueTimeStamps.Top().FireValue = RecordedValue;
		break;

	default:
		break;
	}

}

void UTimeTravelComponent::AllowRecording(bool bInCanRecord)
{
	bShouldRecord = bInCanRecord;
}

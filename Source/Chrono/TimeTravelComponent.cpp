// A prototype by Evangelos ("Vaggelis") Tsesmelidakis. All rights reserved. 2019


#include "TimeTravelComponent.h"

// Default constructor
FTimestampedInputs::FTimestampedInputs()
{
	TimeStamp, JumpValue, StopJumpingValue, MoveForwardValue, MoveRightValue, TurnValue, TurnAtRateValue, LookUpValue, LookUpAtRateValue, FireValue = 0;
}

// Constructor with only a known timestamp
FTimestampedInputs::FTimestampedInputs(float ts)
	:TimeStamp{ ts }
{}

// Constructor to initialize a TimestampedInputs struct with known values
// TODO Vaggelis: decide if I truly need this constructor
FTimestampedInputs::FTimestampedInputs(float ts, float jv, float sjv, float mfv, float mrv, float tv, float tarv, float luv, float luarv, float fv)

	:TimeStamp{ ts },
	JumpValue{ jv },
	StopJumpingValue{ sjv },
	MoveForwardValue{ mfv },
	MoveRightValue{ mrv },
	TurnValue{ tv },
	TurnAtRateValue{ tarv },
	LookUpValue{ luv },
	LookUpAtRateValue{ luarv },
	FireValue{ fv }
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

TArray<FTimestampedInputs> UTimeTravelComponent::GetTimestampedInputs() const
{
	return TimestampedInputs;
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

void UTimeTravelComponent::AddTimestampedInput(float RecordedTimeStamp, EInputActionEnum RecordedActionName, float RecordedValue)
{
	// If this is a non-empty array
	if (TimestampedInputs.Num() > 0)
	{
		// Check the last entry's timestamp
		if (TimestampedInputs.Top().TimeStamp == RecordedTimeStamp)
		{
			// If same then amend the float variables in same entry based on the Enum passed
			AmendTimestampedInputEntry(TimestampedInputs.Top(), RecordedActionName, RecordedValue);
		}
		// If this is a new entry, just add as is
		else
		{
			TimestampedInputs.Add(FTimestampedInputs(RecordedTimeStamp));
			AmendTimestampedInputEntry(TimestampedInputs.Top(), RecordedActionName, RecordedValue);

		}
	}
	// otherwise add a FIRST element
	else
	{
		TimestampedInputs.Add(FTimestampedInputs(RecordedTimeStamp));
		AmendTimestampedInputEntry(TimestampedInputs.Top(), RecordedActionName, RecordedValue);
	}
}

void UTimeTravelComponent::WipeHistory()
{
	// Empty timestamp array so as not compound historic actions
	TimestampedInputs.Empty();
}

void UTimeTravelComponent::AllowRecording(bool bInCanRecord)
{
	bShouldRecord = bInCanRecord;
}

/** Helper functions */

// Helper to update the input float values in a TimestampedInputs entry
void AmendTimestampedInputEntry(FTimestampedInputs& Out_TimestampedInputs, EInputActionEnum RecordedActionName, float RecordedValue)
{
	switch (RecordedActionName)
	{
	case EInputActionEnum::Jump:
		Out_TimestampedInputs.JumpValue = RecordedValue;
		break;

	case EInputActionEnum::Stop_Jumping:
		Out_TimestampedInputs.StopJumpingValue = RecordedValue;
		break;

	case EInputActionEnum::Move_Forward:
		Out_TimestampedInputs.MoveForwardValue = RecordedValue;
		break;

	case EInputActionEnum::Move_Right:
		Out_TimestampedInputs.MoveRightValue = RecordedValue;
		break;

	case EInputActionEnum::Turn:
		Out_TimestampedInputs.TurnValue = RecordedValue;
		break;

	case EInputActionEnum::Turn_At_Rate:
		Out_TimestampedInputs.TurnAtRateValue = RecordedValue;
		break;

	case EInputActionEnum::Look_Up:
		Out_TimestampedInputs.LookUpValue = RecordedValue;
		break;

	case EInputActionEnum::Look_Up_At_Rate:
		Out_TimestampedInputs.LookUpAtRateValue = RecordedValue;
		break;

	case EInputActionEnum::Fire:
		Out_TimestampedInputs.FireValue = RecordedValue;
		break;

	default:
		break;
	}
}



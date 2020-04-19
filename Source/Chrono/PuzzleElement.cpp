// Copyright 2020. All rights reserved. A prototype by CodingRook.


#include "PuzzleElement.h"

// Sets default values for this component's properties
UPuzzleElement::UPuzzleElement()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Default for all trigger actors but can be adjusted via blueprint or child class constructors
	bIsTriggered = false;

	// Default trigger type to none, so as to force a correct choice out of the two types from within derived classes!
	PuzzleElementType = EPuzzleElementType::Type_None;
}

// Called every frame
void UPuzzleElement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UPuzzleElement::IsTriggered() const
{
	return bIsTriggered;
}

void UPuzzleElement::SetElementTriggeredFlag(bool InFlag)
{
	bIsTriggered = InFlag;
}

void UPuzzleElement::SetPuzzleElementType(EPuzzleElementType InElementType)
{
	PuzzleElementType = InElementType;
}

EPuzzleElementType UPuzzleElement::GetPuzzleElementType() const
{
	return EPuzzleElementType();
}

// Called when the game starts
void UPuzzleElement::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}







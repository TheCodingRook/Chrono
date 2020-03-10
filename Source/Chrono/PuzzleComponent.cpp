// Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.


#include "PuzzleComponent.h"
#include "PuzzleElement.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UPuzzleComponent::UPuzzleComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPuzzleComponent::BeginPlay()
{
	Super::BeginPlay();
	/*
	for (auto Actor : ActorsList)
	{
		if (Cast<UPuzzleElement>(Actor->FindComponentByClass(PuzzleElementClass)))
		{
			PuzzleElementsList.Add(Cast<UPuzzleElement>(Actor->FindComponentByClass(PuzzleElementClass)));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("A valid Puzzle Element component was not found in the list of linked actors! for: %s"), *GetOwner()->GetName())
		}
	}*/
	// ...
	
}


// Called every frame
void UPuzzleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TArray<AActor*> UPuzzleComponent::GetActorsList()
{
	return ActorsList;
}

TArray<UPuzzleElement*> UPuzzleComponent::GetPuzzleElementsList() const
{
	return PuzzleElementsList;
}

void UPuzzleComponent::AddToElementsList(UPuzzleElement* PuzzleElement)
{
	PuzzleElementsList.Add(PuzzleElement);
}

bool UPuzzleComponent::bAreAllPuzzleElementsActive()
{
	for (auto PuzzleElement : PuzzleElementsList)
	{
		if (!PuzzleElement->IsTriggered())
		{
			return false;
		}
	}

	// If the loop hasn't failed then return true - all triggers activated!
	return true;
}


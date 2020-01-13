// Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.


#include "TriggerActorComponent.h"
#include "TriggerActorBase.h"

// Sets default values for this component's properties
UTriggerActorComponent::UTriggerActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTriggerActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTriggerActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TArray<ATriggerActorBase*> UTriggerActorComponent::GetTriggerActorsList()
{
	return TriggerActorsList;
}

bool UTriggerActorComponent::AreAllTriggersActive()
{
	for (auto Trigger : TriggerActorsList)
	{
		if (!Trigger->IsTriggered())
		{
			return false;
		}
	}
	
	// If the loop hasn't failed then return true - all triggers activated!
	return true;
}


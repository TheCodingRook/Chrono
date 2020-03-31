// Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.


#include "InteractionComponent.h"
#include "ChronoGameInstance.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInteractionComponent::ExecuteInteraction(AActor* Instigator)
{
	UE_LOG(LogTemp, Warning, TEXT("Command is: %s from owner: %s"), *GetName(), *GetOwner()->GetName())
	// Clear the interaction command register? - probably not for grabbing as it works as a toggle...
	//Cast<UChronoGameInstance>(GetWorld()->GetGameInstance())->DeRegisterCurrentInteractionCommand();
}

// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

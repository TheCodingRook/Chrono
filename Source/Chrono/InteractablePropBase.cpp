// Copyright 2020. All rights reserved. A prototype by CodingRook.


#include "InteractablePropBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "InteractionComponent.h"


// Sets default values
AInteractablePropBase::AInteractablePropBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PropMesh = CreateDefaultSubobject<UStaticMeshComponent>("Prop Mesh");
	PropMesh->SetupAttachment(RootComponent);
	//SetRootComponent(PropMesh); This overrides the name "Prop PropMesh"....

	TextTrigger = CreateDefaultSubobject<USphereComponent>("Text Trigger");
	TextTrigger->SetSphereRadius(100);
	TextTrigger->SetupAttachment(PropMesh);

	// Set up a default value for the name (can help remind that a prop needs an actual name!)
	PropName = FText::FromString("Unnamed");
}

void AInteractablePropBase::SetIsInteractedWith(bool InFlag)
{
	bIsInteractedWith = InFlag;
}

UInteractionComponent* AInteractablePropBase::GetInteractionCommand()
{
	return InteractionCommand;
}

void AInteractablePropBase::OnWasInteractedWith_Implementation()
{
	// To be extended in Blueprint
}

void AInteractablePropBase::OnReEnableInteraction_Implementation()
{
	// To be extended in Blueprint
}


// Called when the game starts or when spawned
void AInteractablePropBase::BeginPlay()
{
	Super::BeginPlay();

	// Set up the interaction component
	if (!InteractionClass)
	{
		UE_LOG(LogTemp, Error, TEXT("You have not specified an interaction component class for: %s"), *GetName())
	}

	else
	{
		InteractionCommand = NewObject<UInteractionComponent>(this, InteractionClass);
		if (InteractionCommand)
		{
			InteractionCommand->RegisterComponent();
		}
	}
	
}

// Called every frame
void AInteractablePropBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


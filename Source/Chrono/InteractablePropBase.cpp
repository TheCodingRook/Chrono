// Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.


#include "InteractablePropBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AInteractablePropBase::AInteractablePropBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PropMesh = CreateAbstractDefaultSubobject<UStaticMeshComponent>("Prop Mesh");
	PropMesh->SetupAttachment(RootComponent);
	//SetRootComponent(PropMesh); This overrides the name "Prop PropMesh"....

	TextTrigger = CreateDefaultSubobject<USphereComponent>("Text Trigger");
	TextTrigger->SetSphereRadius(100);
	TextTrigger->SetupAttachment(PropMesh);

	// Set up a default value for the name (can help remind that a prop needs an actual name!)
	PropName = FText::FromString("Unnamed");
}

// Called when the game starts or when spawned
void AInteractablePropBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractablePropBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


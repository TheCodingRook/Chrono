// Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.


#include "TimePortal.h"
#include "Components\BoxComponent.h"
#include "Components\SceneComponent.h"
#include "Components\StaticMeshComponent.h"
#include "Components\SceneCaptureComponent2D.h"
#include "Components\TextRenderComponent.h"
#include "Components\ArrowComponent.h"
#include "ChronoCharacter.h"

// Sets default values
ATimePortal::ATimePortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Set up the default root component
	DefaultRoot = CreateDefaultSubobject<USceneComponent>("Default root");
	SetRootComponent(DefaultRoot);

	// Set up the bounds volume for the time portal
	CollisionBox = CreateDefaultSubobject<UBoxComponent>("Portal Frame");
	CollisionBox-> SetupAttachment(DefaultRoot);
	CollisionBox->SetWorldScale3D(FVector(2.f, 0.5f, 2.f));
	
	// Set up the mesh to project the portal's view on
	PortalScreen = CreateDefaultSubobject<UStaticMeshComponent>("Portal Screen");
	PortalScreen->SetupAttachment(CollisionBox);
	
	// Setup the spawn point for this time portal and its helper children components
	TeleportLocation = CreateDefaultSubobject<USceneComponent>("Teleport Location");
	TeleportLocation->SetupAttachment(DefaultRoot);
	TeleportLocation->SetRelativeLocation(FVector(0.f, -1000.f, -32.f));

	// Setup a text label for the spawn point so it is visible in the editor (but not in game obviously)
	TeleportLocationText = CreateDefaultSubobject<UTextRenderComponent>("Spawn point label");
	TeleportLocationText->SetupAttachment(TeleportLocation);
	TeleportLocationText->SetRelativeRotation(FRotator(90.f, 90.f, 0.f));
	TeleportLocationText->SetText("Teleport spawn point");
	TeleportLocationText->SetHiddenInGame(true);
	// Do some some default text formatting
	TeleportLocationText->SetWorldSize(50.f);
	TeleportLocationText->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	TeleportLocationText->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);

	// Setup the arrow component for the teleport spawn point
	TeleportLocationArrow = CreateDefaultSubobject<UArrowComponent>("Spawn point arrow indicator");
	TeleportLocationArrow->SetupAttachment(TeleportLocation);
	TeleportLocationArrow->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	TeleportLocationArrow->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	TeleportLocationArrow->SetWorldScale3D(FVector(1.f, 1.f, 1.f));

	// Setup the portal screen
	PortalProjector = CreateDefaultSubobject<USceneCaptureComponent2D>("Portal Projector");
	PortalProjector->SetupAttachment(TeleportLocation);
	PortalProjector->SetRelativeLocation(FVector(0.f, 0.f, 75.f));
	PortalProjector->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
}

void ATimePortal::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	auto OtherCharacter = CastChecked<AChronoCharacter>(OtherActor);

	// If it's our ChronoCharacter that passed through the time portal...
	if (OtherCharacter)
	{
		// ...then broadcast the time travel event...
		OnPortalTraversal.Broadcast();

		// ...and teleport the character to the teleport spawn point
		OtherActor->SetActorLocation(TeleportLocation->GetComponentLocation());
		OtherCharacter->SetActorRotation(TeleportLocationArrow->GetComponentRotation());
		OtherCharacter->GetController()->SetControlRotation(PortalProjector->GetComponentRotation());

	}
	
}

// Called when the game starts or when spawned
void ATimePortal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATimePortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

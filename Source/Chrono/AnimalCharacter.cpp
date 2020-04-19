// Copyright 2020. All rights reserved. A prototype by CodingRook.


#include "AnimalCharacter.h"
#include "GameFramework\CharacterMovementComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AAnimalCharacter::AAnimalCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//MaxAnimalWalkSpeed = GetCharacterMovement()->MaxWalkSpeed; //Set the animal's max walk speed to equal that of default character movement component setting
	//MaxAnimalRunSpeed = 3 * MaxAnimalWalkSpeed; // Just an arbitrary multiple of default walk speed here; should be modified in Blueprint for each animal
	//MinSafetyDistance = 500; //Arbitrary choice for the minumum distance the animal will keep from our character

	//Create a collision sphere to serve as a mouth socket (for detecting eating/biting for example)
	MouthCollision = CreateDefaultSubobject<USphereComponent>("Mouth Collision");
	MouthCollision->SetupAttachment(GetMesh(),"MouthSocket");
}

// Called when the game starts or when spawned
void AAnimalCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAnimalCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



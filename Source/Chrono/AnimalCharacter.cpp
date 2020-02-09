// Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.


#include "AnimalCharacter.h"
#include "GameFramework\CharacterMovementComponent.h"

// Sets default values
AAnimalCharacter::AAnimalCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//MaxAnimalWalkSpeed = GetCharacterMovement()->MaxWalkSpeed; //Set the animal's max walk speed to equal that of default character movement component setting
	//MaxAnimalRunSpeed = 3 * MaxAnimalWalkSpeed; // Just an arbitrary multiple of default walk speed here; should be modified in Blueprint for each animal
	//MinSafetyDistance = 500; //Arbitrary choice for the minumum distance the animal will keep from our character

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



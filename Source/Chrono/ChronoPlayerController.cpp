// Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.


#include "ChronoPlayerController.h"
#include "GameFramework\Character.h"

void AChronoPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent);
	SetUpNewRecordableAction("Grab", IE_Pressed, this, &AChronoPlayerController::Jump);


}

void AChronoPlayerController::Jump()
{
	GetCharacter()->Jump();
}
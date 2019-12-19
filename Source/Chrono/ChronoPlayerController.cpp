// Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.


#include "ChronoPlayerController.h"
#include "GameFramework\Character.h"

// Default constructor not implemented
FActionNameToValueMap::FActionNameToValueMap()
{
}

FActionNameToValueMap::FActionNameToValueMap(FName InActionName, int32 InActionIndex)
	:ActionName{ InActionName },
	ActionIndex{ InActionIndex }
{}

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

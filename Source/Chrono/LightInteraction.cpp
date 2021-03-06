// Copyright 2020. All rights reserved. A prototype by CodingRook.


#include "LightInteraction.h"

ULightInteraction::ULightInteraction()
{
	InteractionText = FText::FromString("Light").ToUpper();
}

void ULightInteraction::ExecuteInteraction(AChronoCharacter* Instigator)
{
	//Not calling Super here because if light interaction is unavailable (e.g. due to lack of a corresponding
	// lit torch) there is no point executing it
	//Super::ExecuteInteraction(Instigator);
	LightFire(Instigator);
}
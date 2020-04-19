// Copyright 2020. All rights reserved. A prototype by CodingRook.


#include "UnlockInteraction.h"

UUnlockInteraction::UUnlockInteraction()
{
	InteractionText = FText::FromString("Unlock").ToUpper();
}

void UUnlockInteraction::ExecuteInteraction(AChronoCharacter* Instigator)
{
	//Not calling Super here because if unlock interaction is unavailable (e.g. due to lack of a corresponding
	// key) there is no point executing it
	//Super::ExecuteInteraction(Instigator);
	UnlockProp(Instigator);
}


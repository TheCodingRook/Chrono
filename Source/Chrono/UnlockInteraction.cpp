// Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.


#include "UnlockInteraction.h"

void UUnlockInteraction::ExecuteInteraction(AActor* Instigator)
{
	Super::ExecuteInteraction(Instigator);
	UnlockProp(Instigator);
}


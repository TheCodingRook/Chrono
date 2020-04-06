// Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.


#include "LightInteraction.h"

void ULightInteraction::ExecuteInteraction(AActor* Instigator)
{
	Super::ExecuteInteraction(Instigator);
	LightFire(Instigator);
}
// Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.


#include "ChronoGameInstance.h"
#include "InteractionComponent.h"

UInteractionComponent* UChronoGameInstance::GetCurrentInteractionCommand()
{
	return CurrentInteractionCommand;
}

void UChronoGameInstance::RegisterCurrentInteractionCommand(UInteractionComponent* InCommand)
{
	CurrentInteractionCommand = InCommand;
}

void UChronoGameInstance::DeRegisterCurrentInteractionCommand()
{
	CurrentInteractionCommand = nullptr;
}

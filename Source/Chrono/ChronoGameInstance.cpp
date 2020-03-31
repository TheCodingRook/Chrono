// Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.


#include "ChronoGameInstance.h"
#include "InteractionComponent.h"

void UChronoGameInstance::PushNewInteractionCommand(UInteractionComponent* NewInteraction)
{
	InteractionCommandStack.Add(NewInteraction);
}

void UChronoGameInstance::PopInteractionCommand(UInteractionComponent* InteractionCommandToRemove)
{
	if (InteractionCommandStack.Num() > 0)
	{
		InteractionCommandStack.RemoveSingle(InteractionCommandToRemove);
	}
}

UInteractionComponent* UChronoGameInstance::GetLatestInteractionCommand()
{
	if (InteractionCommandStack.Num() > 0)
	{
		return InteractionCommandStack.Last();
	}
	else
	{
		return nullptr;
	}
}

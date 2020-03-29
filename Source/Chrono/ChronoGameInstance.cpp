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

void UChronoGameInstance::PushNewInteractionCommand(UInteractionComponent* NewInteraction)
{
	InteractionCommandStack.Add(NewInteraction);
}

UInteractionComponent* UChronoGameInstance::PopInteractionCommand()
{
	if (InteractionCommandStack.Num() > 0)
	{
		return InteractionCommandStack.Pop();
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Attempted to pop from empty InteractionCommandStack!"))
		return nullptr;
	}
}

UInteractionComponent* UChronoGameInstance::GetLatestInteractionCommand()
{
	return InteractionCommandStack.Last();
}

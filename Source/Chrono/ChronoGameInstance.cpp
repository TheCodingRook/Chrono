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

void UChronoGameInstance::PopInteractionCommand(UInteractionComponent* InteractionCommandToRemove)
{
	if (InteractionCommandStack.Num() > 0)
	{
		InteractionCommandStack.RemoveSingle(InteractionCommandToRemove); // TODO Vaggelis: change the signature to void!
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Attempted to pop from empty InteractionCommandStack!"))
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

// Copyright 2020. All rights reserved. A prototype by CodingRook.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHRONO_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void ExecuteInteraction(class AChronoCharacter* Instigator);

	UFUNCTION(BlueprintPure, Category = "Widget")
	FText GetInteractionText();

	UFUNCTION(BlueprintPure, Category = "Widget")
	FText GetAlternativeInteractionText();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;	

	// What is the main text that this interaction prompts the player with?
	// Remember to set this directly in Blueprint if not already set in a derived C++ class!
	// (i.e. if you create a Blueprint directly inheriting from this class)
	UPROPERTY(VisibleAnywhere, Category = "Widget")
	FText InteractionText;

	UPROPERTY(VisibleAnywhere, Category = "Widget")
	FText AlternativeInteractionText;
};

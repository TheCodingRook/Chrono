// Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PuzzleComponent.generated.h"

class UPuzzleElement;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHRONO_API UPuzzleComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPuzzleComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Return the array of Actors that this owning actor is linked to
	UFUNCTION(BlueprintPure, Category = "Puzzle Elements")
	TArray<AActor*> GetActorsList();

	// Return the array of TriggerActors that the owning actor is linked to
	UFUNCTION(BlueprintPure, Category = "Puzzle Elements")
	TArray<UPuzzleElement*> GetPuzzleElementsList() const;

	// Add to the PuzzleElementsList
	UFUNCTION(BlueprintCallable, Category = "Puzzle Elements")
	void AddToElementsList(UPuzzleElement* PuzzleElement);

	// Check to see if all the triggers for the owning actor are satisfied
	UFUNCTION(BlueprintPure, Category = "Puzzle Elements")
	bool bAreAllPuzzleElementsActive();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// List of actors that will be puzzle elements ("triggers") linked to another actor that represents the puzzle
	UPROPERTY(EditAnywhere, Category = "Puzzle Elements")
	TArray<AActor*> ActorsList;
	
	// List of puzzle element object references from the above actors list
	TArray<UPuzzleElement*> PuzzleElementsList;

	TSubclassOf<UPuzzleElement> PuzzleElementClass;
};

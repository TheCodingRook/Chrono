// Copyright 2020. All rights reserved. A prototype by CodingRook.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PuzzleElement.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPuzzleElementTriggered);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPuzzleElementUnTriggered);

UENUM(BlueprintType)
enum class EPuzzleElementType : uint8
{
	NOSwitch UMETA(DisplayName = "Normally Open Switch"),
	Toggle UMETA(DisplayName = "Toggle Switch"),
	Type_None UMETA(DisplayName = "Null type")
};

/**
 * A generic base component class to implement actors ("puzzle elements") that trigger other switches or other events with architectural elements in the world
 */

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHRONO_API UPuzzleElement : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPuzzleElement();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetElementTriggeredFlag(bool InFlag);

	// Delegate to notify the Actor which this compoenent's owner is linked with, that this PuzzleElement has triggered
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Delegates")
	FOnPuzzleElementTriggered OnPuzzleElementTriggered;

	// Delegate to notify the Actor which this compoenent's owner is linked with, that this PuzzleElement has been triggered as off
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Delegates")
	FOnPuzzleElementUnTriggered OnPuzzleElementUnTriggered;

	UFUNCTION(BlueprintPure, Category = "Element Type")
		bool IsTriggered() const;

	// Trigger type should ideally be set by the constructor, but perhaps useful to include a "setter" just in case
	UFUNCTION(BlueprintCallable, Category = "Element Type")
	void SetPuzzleElementType(EPuzzleElementType InElementType);

	UFUNCTION(BlueprintPure, Category = "ElementType")
	EPuzzleElementType GetPuzzleElementType() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// The main boolean that determines if this TriggerActor is activated or not
	UPROPERTY(VisibleAnywhere, Category = "Triggers")
		bool bIsTriggered;

	// The type of puzzle element this is: toggle (aka on/off) switch, or a Normally Open switch (aka push-to-make, meaning it has to be kept pressed or triggered to be on and turns off when released)
	EPuzzleElementType PuzzleElementType;
};

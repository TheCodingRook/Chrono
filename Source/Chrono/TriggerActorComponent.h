// Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TriggerActorComponent.generated.h"

class ATriggerActorBase;

UCLASS( ClassGroup=(TriggerActors), meta=(BlueprintSpawnableComponent) )
class CHRONO_API UTriggerActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTriggerActorComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Return the array of TriggerActors that the owning actor is linked to
	UFUNCTION(BlueprintPure, Category = "Triggers")
	TArray<ATriggerActorBase*> GetTriggerActorsList();

	// Check to see if all the triggers for the owning actor are satisfied
	UFUNCTION(BlueprintPure, Category = "Triggers")
	bool AreAllTriggersActive();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Triggers")
	TArray<ATriggerActorBase*> TriggerActorsList;
};

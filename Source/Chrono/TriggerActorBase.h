// Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggerActorBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActorTriggered);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActorUnTriggered);

UENUM(BlueprintType)
enum class ETriggerType : uint8
{
	NOSwitch UMETA(DisplayName = "Normally Open Switch"),
	Toggle UMETA(DisplayName = "Toggle Switch"), 
	Type_None UMETA(DisplayName = "Null type")
};

/**
 * A generic base class to implement actors that trigger other switches or other events with architectural elements in the world
 */
UCLASS()
class CHRONO_API ATriggerActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATriggerActorBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Triggers")
	void SetTriggerFlag(bool InFlag);

	// Delegate to notify the Actor that this TriggerActor is linked with, that TriggerActor has triggered
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Triggers")
	FOnActorTriggered OnActorTriggered;

	// Delegate to notify the Actor that this TriggerActor is linked with, that TriggerActor has been triggered off
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Triggers")
	FOnActorUnTriggered OnActorUnTriggered;

	UFUNCTION(BlueprintPure, Category = "Triggers")
	bool IsTriggered() const;

	// Trigger type should ideally be set by the constructor, but perhaps useful to include a "setter" just in case
	UFUNCTION(BlueprintCallable, Category = "Triggers")
	void SetTriggerType(ETriggerType InTriggerType);

	UFUNCTION(BlueprintPure, Category = "Triggers")
	ETriggerType GetTriggerType() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// The main boolean that determines if this TriggerActor is activated or not
	UPROPERTY(VisibleAnywhere, Category = "Triggers")
	bool bIsTriggered;

	// The type of trigger this is: toggle (aka on/off) switch, or a Normally Open switch (aka push-to-make, meaning it has to be kept pressed or triggered to be on and turns off when released)
	ETriggerType TriggerType;
};

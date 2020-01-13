// Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.


#include "TriggerActorBase.h"

// Sets default values
ATriggerActorBase::ATriggerActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Default for all trigger actors but can be adjusted via blueprint or child class constructors
	bIsTriggered = false;

	// Default trigger type to none, so as to force a correct choice out of the two types from within derived classes!
	TriggerType = ETriggerType::Type_None;
}

// Called every frame
void ATriggerActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATriggerActorBase::SetTriggerFlag(bool InFlag)
{
	bIsTriggered = InFlag;
}

bool ATriggerActorBase::IsTriggered() const
{
	return bIsTriggered;
}

void ATriggerActorBase::SetTriggerType(ETriggerType InTriggerType)
{
	TriggerType = InTriggerType;
}

ETriggerType ATriggerActorBase::GetTriggerType() const
{
	return TriggerType;
}

// Called when the game starts or when spawned
void ATriggerActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

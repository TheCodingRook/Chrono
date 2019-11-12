// A prototype by Evangelos ("Vaggelis") Tsesmelidakis. All rights reserved. 2019


#include "BatteryPickup.h"

ABatteryPickup::ABatteryPickup()
{
    GetMesh()->SetSimulatePhysics(true);
	
	// the base power level of the battery
	BatteryPower = 150.f;
}

void ABatteryPickup::WasCollected_Implementation()
{
	// Uae the base pickup behavior
	Super::WasCollected_Implementation();
	// Destroy the battery
	Destroy();
}

// Report the battery power level
float ABatteryPickup::GetPower()
{
	return BatteryPower;
}

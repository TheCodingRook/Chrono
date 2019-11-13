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
	
	
	// Use the base pickup behavior
	Super::WasCollected_Implementation();
	
}

// Report the battery power level
float ABatteryPickup::GetPower() const
{
	return BatteryPower;
}

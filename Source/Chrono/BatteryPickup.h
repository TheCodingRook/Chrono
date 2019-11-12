// A prototype by Evangelos ("Vaggelis") Tsesmelidakis. All rights reserved. 2019

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "BatteryPickup.generated.h"

/**
 * Class that handles battery pickups
 */
UCLASS()
class CHRONO_API ABatteryPickup : public APickup
{
	GENERATED_BODY()
	
public:
	ABatteryPickup();
	
	// Override WasCollected function - use Implementation because it's a Bluepring Native Event
	void WasCollected_Implementation() override;
	
	/** Public accessor function for the BatteryPower float */
	float GetPower();
		
protected:
		/** Set the amount of power the battery gives to the character */
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", meta = (BlueprintProtected = "true"))
		float BatteryPower;
};
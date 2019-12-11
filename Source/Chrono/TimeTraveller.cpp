// A prototype by Evangelos ("Vaggelis") Tsesmelidakis. All rights reserved. 2019


#include "TimeTraveller.h"
#include "TimeTravelComponent.h"

ATimeTraveller::ATimeTraveller()
{
	//Super(); TODO: Do I need to call Super?

	TimeTravel = CreateDefaultSubobject<UTimeTravelComponent>(FName("Time Travel Component"));
}

UTimeTravelComponent* ATimeTraveller::GetTimeTravelComponent() const
{
	return TimeTravel;
}



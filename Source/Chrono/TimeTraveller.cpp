// A prototype by Evangelos ("Vaggelis") Tsesmelidakis. All rights reserved. 2019


#include "TimeTraveller.h"
#include "HistoryRecorder.h"

ATimeTraveller::ATimeTraveller()
{
	//Super(); TODO: Do I need to call Super?

	TimeTravelComponent = CreateDefaultSubobject<UHistoryRecorder>(FName("Time Travel Component"));
}

UHistoryRecorder* ATimeTraveller::GetTimeTravelComponent() const
{
	return TimeTravelComponent;
}



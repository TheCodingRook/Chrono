// A prototype by Evangelos ("Vaggelis") Tsesmelidakis. All rights reserved. 2019

#pragma once

#include "CoreMinimal.h"
#include "ChronoCharacter.h"
#include "TimeTravelComponent.h"
#include "TimeTraveller.generated.h"

/**
 * Custom player character class that implements a "time travel" component
 */
UCLASS()
class CHRONO_API ATimeTraveller : public AChronoCharacter
{
	GENERATED_BODY()

	ATimeTraveller();

public:
	UTimeTravelComponent* GetTimeTravelComponent() const;

	// Very dirty solution to get the replay history loop working
	// these variables are global here so I can call ReplayAction 
	// function as void repeatedely based on an FTimer handle; that's because
	// FTimer's "SetTimer" method requires a function returning void!

	EInputActionEnum TempActionName;
	float TempActionValue;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);
	// End of APawn interface

	/*	Recording input action functions
	*	Execute the typical action first but also record it for playback
	*/

	void JumpAndRecord();
	void StopJumpingAndRecord();

	void MoveForwardAndRecord(float Value);
	void MoveRightAndRecord(float Value);
	
	void TurnAndRecord(float Value);
	void TurnAtRateAndRecord(float Value);
	void LookUpAndRecord(float Value);
	void LookUpAtRateAndRecord(float Value);

	void FireAndRecord();

	// Replay history
	UFUNCTION(BlueprintCallable, Category = "Time Travel", meta = (AllowPrivateAccess = "true"))
	void ReplayHistory();

	void ReplayAction();

private:
	/** Component to implement character's time-travelling ability	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time Travel", meta = (AllowPrivateAccess = "true"))
	UTimeTravelComponent* TimeTravel;

	
};

//  Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TimeTravelComponent.h" // Have to include this here because I cannot forward declare the FUniqueTimeStamp struct in the ReplayPastAction method declaration below
#include "ChronoCharacter.generated.h"

UCLASS(config=Game)
class AChronoCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	AChronoCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** Returns TimeTravelComponent subobject **/
	UFUNCTION(BlueprintPure, Category = "Time Travel")
	FORCEINLINE class UTimeTravelComponent* GetTimeTravelComponent() const { return TimeTravel; }

protected:

	/**	Methods to call to perform movements (and actions when necessary) but also record them for replay in the future */
	void JumpAndRecord();
	void StopJumpingAndRecord();
	void MoveForwardAndRecord(float Value);
	void MoveRightAndRecord(float Value);
	void TurnAndRecord(float Value);
	void TurnAtRateAndRecord(float Rate);
	void LookUpAndRecord(float Value);
	void LookUpAtRateAndRecord(float Rate);

	///////////////////////////////////////////////////////////////////////////////////////////////////
	// LEGACY METHODS THAT ARE NOT BEING USED AT THE MOMENT
	// ************************************************************************************************

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	// END OF LEGACY METHODS ************************************************************************
	//////////////////////////////////////////////////////////////////////////////////////////////////

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	UFUNCTION(BlueprintCallable, Category = "Time Travel", meta = (AllowPrivateAccess = "true"))
	void ReplayPastAction(FUniqueTimeStamp ActionToReplay);

private:
	/** Component to implement character's time-travelling ability	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time Travel", meta = (AllowPrivateAccess = "true"))
	UTimeTravelComponent* TimeTravel;
};

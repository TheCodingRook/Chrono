// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TimeTravelComponent.h"
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

protected:

	/**	Called for jumping */
	void JumpAndRecord();

	/**	Called to stop jumping */
	void StopJumpingAndRecord();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** Called via pawn interface to turn (after recording input first) */
	void Turn(float Value);
	
	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/** Called via pawn interface to turn (after recording input first) */
	void LookUp(float Value);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	// APawn interface
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	// Replay history
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Time Travel", meta = (AllowPrivateAccess = "true"))
	void ReplayHistory();

	void ReplayAction(FRecordedInputAction ActionToReplay);

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE class UTimeTravelComponent* GetTimeTravelComponent() const { return TimeTravel; }

private:
	/** Component to implement character's time-travelling ability	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time Travel", meta = (AllowPrivateAccess = "true"))
	UTimeTravelComponent* TimeTravel;
};


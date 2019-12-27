//  Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TimeTravelComponent.h" // Have to include this here because I cannot forward declare the FTimestampedInputs struct in the ReplayPastActions method declaration below
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

	/** Aim weapon camera location, represented by an arrow component*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* AimCameraLocation;

	/** Set up the TimeWeapon reference and class */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons", meta = (AllowPrivateAccess = "true"))
	class ATimeWeapon* TimeWeapon;
	UPROPERTY(EditDefaultsOnly, Category = "Weapons", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ATimeWeapon> TimeWeaponClass;

	// Bool to track when player has pressed the unholster/holster button that will eventually notify an animation 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Controls", meta = (AllowPrivateAccess = "true"))
	bool bHolsterButtonDown;

	// Bool to track when player has pressed the Aim button that will switch between the cameras 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Controls", meta = (AllowPrivateAccess = "true"))
	bool bAimButtonDown;

	// Vector to calculate aim camera's offset from original position
	FVector CameraOffset;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	float Health;

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

	/** Implements grabbing objects via the physics handle component if one is present*/
	UFUNCTION(BlueprintCallable, Category = "Grabbing")
	void Grab();
	UFUNCTION(BlueprintCallable, Category = "Grabbing")
	void EndGrab();

	// Getter for the Holster/Unholster  boolean
	UFUNCTION(BlueprintPure, Category = "Weapon Controls")
	bool GetHolsterButtonDown() const;

	// Setter for the Holster/Unholster boolean
	UFUNCTION(BlueprintCallable, Category = "WeaponControls")
		void SetHolsterButtonDown(bool InFlag);

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void EquipWeapon();
	UFUNCTION(BlueprintCallable, Category = "Weapons")
		void UnEquipWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void ToggleCameras();

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void ToggleAimButtonDown();

	// Getter for the aim button  boolean
	UFUNCTION(BlueprintPure, Category = "Weapon Controls")
	bool GetAimButtonDown() const;

	// Setter for the aim button boolean
	UFUNCTION(BlueprintCallable, Category = "WeaponControls")
		void SetAimButtonDown(bool InFlag);

	UFUNCTION(BlueprintCallable, Category = "WeaponControls")
	void Fire();

	// Getter for character's health
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealth() const;

	// Setter for character's health
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetHealth(float NewHealthAmount);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void ReduceHealth(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

protected:
	virtual void BeginPlay() override;

	/**	Methods to call to perform movements (and actions when necessary) but also record them for replay in the future */
	//void JumpAndRecord();
	//void StopJumpingAndRecord();
	//void MoveForwardAndRecord(float Value);
	//void MoveRightAndRecord(float Value);
	//void TurnAndRecord(float Value);
	//void TurnAtRateAndRecord(float Rate);
	//void LookUpAndRecord(float Value);
	//void LookUpAtRateAndRecord(float Rate);

	///////////////////////////////////////////////////////////////////////////////////////////////////
	// LEGACY METHODS THAT ARE NOT BEING USED AT THE MOMENT
	// ************************************************************************************************

	/** Resets HMD orientation in VR. */
	//void OnResetVR();

	/** Handler for when a touch input begins. */
	//void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	//void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	// END OF LEGACY METHODS ************************************************************************
	//////////////////////////////////////////////////////////////////////////////////////////////////

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	UFUNCTION(BlueprintCallable, Category = "Time Travel", meta = (AllowPrivateAccess = "true"))
	void ReplayPastActions(FTimestampedInputs ActionsToReplay);
};

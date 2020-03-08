//  Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TimeTravelComponent.h" // Have to include this here because I cannot forward declare the FTimestampedActions struct in the ReplayPastActions method declaration below
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

	// Bool to track of whether player is armed or not 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Controls", meta = (AllowPrivateAccess = "true"))
	bool bHasEquippedWeapon;

	// Bool to track when player has pressed the Aim button that will switch between the cameras 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Controls", meta = (AllowPrivateAccess = "true"))
	bool bAimButtonDown;

	// Bool to track when player has pressed the Grab button so it can determine whether to grab a prop or drop an existing held one
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actions", meta = (AllowPrivateAccess = "true"))
	bool bGrabButtonDown;

	// Vector to calculate aim camera's offset from original position
	FVector CameraOffset;

	// Float to calcuate the camera boom's target length offset from original position
	float BoomOffset;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	float Health;

	// Store pointer to this Character's ChronoPlayerController - if any
	class AChronoPlayerController* MyChronoController;

	// Store pointer to this Character's PlastSelfController - if any
	class APastSelfController* MyPastSelfController;

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

	/* Returns this characther's controller reference*/
	UFUNCTION(BlueprintPure, Category = "Controller")
	AChronoPlayerController* GetMyChronoController() const;

	/* Returns this characther's PastSelfController reference*/
	UFUNCTION(BlueprintPure, Category = "Controller")
	APastSelfController* GetMyPastSelfController() const;

	/* Sets this characther's ChronoPlayerController reference*/
	UFUNCTION(BlueprintCallable, Category = "Controller")
	void SetMyChronoController(AChronoPlayerController* ControllerToSet);

	/* Sets this characther's MyPastSelfController reference*/
	UFUNCTION(BlueprintCallable, Category = "Controller")
	void SetMyPastSelfController(APastSelfController* ControllerToSet);

	/* Returns this characther's TimeWeapon reference*/
	UFUNCTION(BlueprintPure, Category = "Weapons")
	ATimeWeapon* GetTimeWeapon() const;

	/** Implements grabbing objects either by attaching them to a socket or otherwise via the physics handle component (if one is present)*/
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

	UFUNCTION(BlueprintCallable, Category = "Weapons Controls")
	void EquipWeapon();
	UFUNCTION(BlueprintCallable, Category = "Weapons Controls")
	void UnEquipWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapons Controls")
	void ToggleCameras();

	UFUNCTION(BlueprintCallable, Category = "Weapons Controls")
	void ToggleAimButtonDown();

	// Getter for the aim button  boolean
	UFUNCTION(BlueprintPure, Category = "Weapon Controls")
	bool GetAimButtonDown() const;

	// Setter for the aim button boolean
	UFUNCTION(BlueprintCallable, Category = "Weapon Controls")
	void SetAimButtonDown(bool InFlag);

	// Getter for the character armed boolean
	UFUNCTION(BlueprintPure, Category = "Weapon Controls")
	bool GetHasEquippedWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "Weapon Controls")
	void Fire();

	// Getter for the Grab/Drop prop  boolean
	UFUNCTION(BlueprintPure, Category = "Grabbing")
	bool GetGrabButtonDown() const;

	// Setter for the Grab/Drop prop boolean
	UFUNCTION(BlueprintCallable, Category = "Grabbing")
	void SetGrabButtonDown(bool InFlag);
	
	// Getter for character's health
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealth() const;

	// Setter for character's health
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetHealth(float NewHealthAmount);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void ReduceHealth(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void KillCharacter();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Time Travel")
	void OnTravelBackInTime();

protected:
	virtual void BeginPlay() override;
};

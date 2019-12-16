// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ChronoCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// AChronoCharacter

AChronoCharacter::AChronoCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	// Sets up a time travel component
	TimeTravel = CreateDefaultSubobject<UTimeTravelComponent>(FName("Time Travel Component"));
}

//////////////////////////////////////////////////////////////////////////
// Input

void AChronoCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings, making every movement and action recordable; all actions implemented locally
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AChronoCharacter::JumpAndRecord); // cannot override Pawn's Jump here so had to name differently
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AChronoCharacter::StopJumpingAndRecord);

	PlayerInputComponent->BindAxis("MoveForward", this, &AChronoCharacter::MoveForwardAndRecord);
	PlayerInputComponent->BindAxis("MoveRight", this, &AChronoCharacter::MoveRightAndRecord);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick

	PlayerInputComponent->BindAxis("Turn", this, &AChronoCharacter::TurnAndRecord);
	PlayerInputComponent->BindAxis("TurnRate", this, &AChronoCharacter::TurnAtRateAndRecord);
	PlayerInputComponent->BindAxis("LookUp", this, &AChronoCharacter::LookUpAndRecord);
	PlayerInputComponent->BindAxis("LookUpAtRate", this, &AChronoCharacter::LookUpAtRateAndRecord);


	/* TOUCH DEVICES AND VR HEADSET NOT IMPLEMENTED YET */
	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AChronoCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AChronoCharacter::TouchStopped);
	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AChronoCharacter::OnResetVR);
}

void AChronoCharacter::JumpAndRecord()
{
		if (Controller && TimeTravel)
		{
			if (TimeTravel->ShouldRecord())
			{
				// First record time and identity of input...
				TimeTravel->AddRecordedAction(GetWorld()->GetTimeSeconds(), EInputActionEnum::Jump, 1);
			}

			// ... then execute action through APawn's interface
			Jump();
		}
}

void AChronoCharacter::StopJumpingAndRecord()
{
	if (Controller && TimeTravel)
	{
		if (TimeTravel->ShouldRecord())
		{
			// First record time and identity of input...
			TimeTravel->AddRecordedAction(GetWorld()->GetTimeSeconds(), EInputActionEnum::Stop_Jumping, 0);
		}
		// ... then execute action through APawn's interface
		StopJumping();
		
	}
}

void AChronoCharacter::MoveForwardAndRecord(float Value)
{
	if (Controller && TimeTravel)
	{
		if (TimeTravel->ShouldRecord())
		{
			// First record time and identity of input...
			TimeTravel->AddRecordedAction(GetWorld()->GetTimeSeconds(), EInputActionEnum::Move_Forward, Value);
		}

		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AChronoCharacter::MoveRightAndRecord(float Value)
{
	if (Controller && TimeTravel) // TODO: Vaggelis: Earlier versions of this had a Value!= 0.0f test... consider this for smaller-size array?
	{
		if (TimeTravel->ShouldRecord())
		{
			// First record time and identity of input...
			TimeTravel->AddRecordedAction(GetWorld()->GetTimeSeconds(), EInputActionEnum::Move_Right, Value);
		}

		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AChronoCharacter::TurnAndRecord(float Value)
{
	if (Controller && TimeTravel)
	{
		if (TimeTravel->ShouldRecord())
		{
			// First record time and identity of input...
			TimeTravel->AddRecordedAction(GetWorld()->GetTimeSeconds(), EInputActionEnum::Turn, Value);
		}

		// call Pawn's interface to turn
		AddControllerYawInput(Value);
	}
}

void AChronoCharacter::TurnAtRateAndRecord(float Rate)
{
	if (Controller && TimeTravel)
	{
		if (TimeTravel->ShouldRecord())
		{
			// First record time and identity of input...
			TimeTravel->AddRecordedAction(GetWorld()->GetTimeSeconds(), EInputActionEnum::Turn_At_Rate, Rate);
		}

		// calculate delta for this frame from the rate information
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void AChronoCharacter::LookUpAndRecord(float Value)
{
	if (Controller && TimeTravel)
	{
		if (TimeTravel->ShouldRecord())
		{
			// First record time and identity of input...
			TimeTravel->AddRecordedAction(GetWorld()->GetTimeSeconds(), EInputActionEnum::Look_Up, Value);
		}

		// call Pawn's interface to look up
		AddControllerPitchInput(Value);
	}
}

void AChronoCharacter::LookUpAtRateAndRecord(float Rate)
{
	if (Controller && TimeTravel)
	{
		if (TimeTravel->ShouldRecord())
		{
			// First record time and identity of input...
			TimeTravel->AddRecordedAction(GetWorld()->GetTimeSeconds(), EInputActionEnum::Look_Up_At_Rate, Rate);
		}

		// calculate delta for this frame from the rate information
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// LEGACY METHODS THAT ARE NOT BEING USED AT THE MOMENT
// ************************************************************************************************
void AChronoCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AChronoCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AChronoCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}
// END OF LEGACY METHODS ************************************************************************
//////////////////////////////////////////////////////////////////////////////////////////////////


void AChronoCharacter::ReplayPastAction(FUniqueTimeStamp ActionToReplay)
{
		if (ActionToReplay.JumpValue == 1.0f)
		{
			Jump(); // This is ACharacter's interface
		}

	if (ActionToReplay.StopJumpingValue == 1.0f)
	{
		StopJumping(); // This is ACharacter's interface
	}
	if (ActionToReplay.MoveForwardValue != 0.0f)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, ActionToReplay.MoveForwardValue);
	}


	if (ActionToReplay.MoveRightValue != 0.0f)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, ActionToReplay.MoveRightValue);
	}

	if (ActionToReplay.TurnValue != 0.0f)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is turning with value: %f"), *GetName(), ActionToReplay.TurnValue)
		AddControllerYawInput(ActionToReplay.TurnValue); // This is APawn's interface
	}

	if (ActionToReplay.TurnAtRateValue != 0.0f)
	{
		AddControllerYawInput(ActionToReplay.TurnAtRateValue * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}

	if (ActionToReplay.LookupValue != 0.0f)
	{
		AddControllerPitchInput(ActionToReplay.LookupValue); // This is APawn's interface
	}

	if (ActionToReplay.LookUpAtRateValue != 0.0f)
	{
		AddControllerPitchInput(ActionToReplay.LookUpAtRateValue * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}

	if (ActionToReplay.FireValue == 1.0f)
	{
		// Fire implementation here
	}

}

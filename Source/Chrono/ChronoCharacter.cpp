// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ChronoCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "TimerManager.h"

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
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AChronoCharacter::JumpAndRecord); // cannot override Pawn's Jump here so had to name differently
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AChronoCharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AChronoCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AChronoCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick

	PlayerInputComponent->BindAxis("Turn", this, &AChronoCharacter::Turn);
	PlayerInputComponent->BindAxis("TurnRate", this, &AChronoCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &AChronoCharacter::LookUp);
	PlayerInputComponent->BindAxis("LookUpAtRate", this, &AChronoCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AChronoCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AChronoCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AChronoCharacter::OnResetVR);
}

void AChronoCharacter::JumpAndRecord()
{
	if (Controller)
	{
		if (TimeTravel->ShouldRecord())
		{
			// First record time and identity of input...
			TimeTravel->AddRecordedAction(GetWorld()->GetTimeSeconds(), EInputActionEnum::Jump, 1);

			// ... then execute action through APawn's interface
			Jump();
		}
	}
}

void AChronoCharacter::StopJumpingAndRecord()
{
	if (Controller)
	{
		if (TimeTravel->ShouldRecord())
		{
			// First record time and identity of input...
			TimeTravel->AddRecordedAction(GetWorld()->GetTimeSeconds(), EInputActionEnum::Stop_Jumping, 0);

			// ... then execute action through APawn's interface
			StopJumping();
		}
	}
}

void AChronoCharacter::MoveForward(float Value)
{
	if (Controller)
	{
		if (TimeTravel->ShouldRecord())
		{
			// First record time and identity of input...
			TimeTravel->AddRecordedAction(GetWorld()->GetTimeSeconds(), EInputActionEnum::Move_Forward, Value);


			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value);
		}
	}
}

void AChronoCharacter::MoveRight(float Value)
{
	if (Controller) // TODO: Vaggelis: Earlier versions of this had a Value!= 0.0f test... consider this for smaller-size array?
	{
		if (TimeTravel->ShouldRecord())
		{
			// First record time and identity of input...
			TimeTravel->AddRecordedAction(GetWorld()->GetTimeSeconds(), EInputActionEnum::Move_Right, Value);

			// find out which way is right
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			// add movement in that direction
			AddMovementInput(Direction, Value);
		}
	}
}

void AChronoCharacter::Turn(float Value)
{
	if (Controller)
	{
		if (TimeTravel->ShouldRecord())
		{
			// First record time and identity of input...
			TimeTravel->AddRecordedAction(GetWorld()->GetTimeSeconds(), EInputActionEnum::Turn, Value);

			// call Pawn's interface to turn
			AddControllerYawInput(Value);
		}
	}
}

void AChronoCharacter::TurnAtRate(float Rate)
{
	if (Controller)
	{
		if (TimeTravel->ShouldRecord())
		{
			// First record time and identity of input...
			TimeTravel->AddRecordedAction(GetWorld()->GetTimeSeconds(), EInputActionEnum::Turn_At_Rate, Rate);

			// calculate delta for this frame from the rate information
			AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
		}
	}
}

void AChronoCharacter::LookUp(float Value)
{
	if (Controller)
	{
		if (TimeTravel->ShouldRecord())
		{
			// First record time and identity of input...
			TimeTravel->AddRecordedAction(GetWorld()->GetTimeSeconds(), EInputActionEnum::Look_Up, Value);

			// call Pawn's interface to look up
			AddControllerPitchInput(Value);
		}
	}
}

void AChronoCharacter::LookUpAtRate(float Rate)
{
	if (Controller)
	{
		if (TimeTravel->ShouldRecord())
		{
			// First record time and identity of input...
			TimeTravel->AddRecordedAction(GetWorld()->GetTimeSeconds(), EInputActionEnum::Look_Up, Rate);

			// calculate delta for this frame from the rate information
			AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
		}
	}
}


void AChronoCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AChronoCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AChronoCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AChronoCharacter::ReplayHistory()
{
	// Stop recording input actions because "time travel" has been initiated
	TimeTravel->AllowRecording(false);

	// To be used for calculating time elapsed between recorded actions.
	float PreviousTimeStamp = 0;

	FTimerHandle Delay;

	// manual for loop to go through the array of structs with delays.
	int i = 0;

	while (i < TimeTravel->GetPastActions().Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Array index: %i"), i)
			TempActionName = TimeTravel->GetPastActions()[i].ActionName;
			TempActionValue = TimeTravel->GetPastActions()[i].Value;

		const UEnum* InputActionEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EInputActionEnum"));

		UE_LOG(LogTemp, Warning, TEXT("Action is: %s"), *InputActionEnum->GetEnumName((int32)TempActionName))
			UE_LOG(LogTemp, Warning, TEXT("TimeStamp is: %f"), TempActionValue)

			// Deal with the first element in the array when time travel begins.
			if (i == 0)
			{
				// Execute immediately; this is the first index in the array
				GetWorldTimerManager().SetTimer(Delay, this, &AChronoCharacter::ReplayAction, 0.001f, false);
			}
			else
			{
				GetWorldTimerManager().SetTimer(Delay, this, &AChronoCharacter::ReplayAction, TimeTravel->GetPastActions()[i].TimeStamp - TimeTravel->GetPastActions()[i - 1].TimeStamp, false);
			}
		i++;
	}
}

void AChronoCharacter::ReplayAction()
{
	UE_LOG(LogTemp, Warning, TEXT("Entered ReplayAction()"))
		switch (TempActionName)
		{
		case EInputActionEnum::Jump:
			Jump(); // This is ACharacter's interface
			break;

		case EInputActionEnum::Stop_Jumping:
			StopJumping(); // This is ACharacter's interface
			break;

		case EInputActionEnum::Move_Forward:
			MoveForward(TempActionValue);
			break;

		case EInputActionEnum::Move_Right:
			MoveRight(TempActionValue);
			break;

		case EInputActionEnum::Turn:
			AddControllerYawInput(TempActionValue); // This is APawn's interface
			break;

		case EInputActionEnum::Turn_At_Rate:
			TurnAtRate(TempActionValue);
			break;

		case EInputActionEnum::Look_Up:
			AddControllerPitchInput(TempActionValue); // This is APawn's interface
			break;

		case EInputActionEnum::Look_Up_At_Rate:
			LookUpAtRate(TempActionValue);
			break;

		case EInputActionEnum::Fire:

		default:
			break;
		}
}

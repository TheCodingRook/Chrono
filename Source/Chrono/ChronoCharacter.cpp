// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ChronoCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ChronoPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "GrabbingAbility.h"
#include "TimeWeapon.h"


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

	// Initialize the holstertoggle boolean
	bHolsterButtonDown = false;
}

void AChronoCharacter::Grab()
{
	UGrabbingAbility* Grabber = FindComponentByClass<UGrabbingAbility>();
	if (Grabber != nullptr)
	{
		Grabber->GrabObject();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PhysicsHandleComponent not found for Character: %s"), *GetName())
	}
}

void AChronoCharacter::EndGrab()
{
	UGrabbingAbility* Grabber = FindComponentByClass<UGrabbingAbility>();
	if (Grabber != nullptr)
	{
		Grabber->DropObject();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PhysicsHandleComponent not found for Character: %s"), *GetName())
	}
}

bool AChronoCharacter::GetHolsterButtonDown() const
{
	return bHolsterButtonDown;
}

void AChronoCharacter::SetHolsterButtonDown(bool InFlag)
{
	bHolsterButtonDown = InFlag;
}

void AChronoCharacter::EquipWeapon()
{
	TimeWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), "WeaponSocket");
}

void AChronoCharacter::UnEquipWeapon()
{
	TimeWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), "WeaponHolster");
}

//////////////////////////////////////////////////////////////////////////
// Input

void AChronoCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings, making every movement and action recordable; all actions implemented locally
	check(PlayerInputComponent);
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AChronoCharacter::JumpAndRecord); // cannot override Pawn's Jump here so had to name differently
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &AChronoCharacter::StopJumpingAndRecord);

	//PlayerInputComponent->BindAxis("MoveForward", this, &AChronoCharacter::MoveForwardAndRecord);
	//PlayerInputComponent->BindAxis("MoveRight", this, &AChronoCharacter::MoveRightAndRecord);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick

	//PlayerInputComponent->BindAxis("Turn", this, &AChronoCharacter::TurnAndRecord);
	//PlayerInputComponent->BindAxis("TurnRate", this, &AChronoCharacter::TurnAtRateAndRecord);
	//PlayerInputComponent->BindAxis("LookUp", this, &AChronoCharacter::LookUpAndRecord);
	//PlayerInputComponent->BindAxis("LookUpAtRate", this, &AChronoCharacter::LookUpAtRateAndRecord);


	/* TOUCH DEVICES AND VR HEADSET NOT IMPLEMENTED YET */
	// handle touch devices
	//PlayerInputComponent->BindTouch(IE_Pressed, this, &AChronoCharacter::TouchStarted);
	//PlayerInputComponent->BindTouch(IE_Released, this, &AChronoCharacter::TouchStopped);
	// VR headset functionality
	//PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AChronoCharacter::OnResetVR);
}
/*
void AChronoCharacter::JumpAndRecord()
{
		if (Controller && TimeTravel)
		{
			if (TimeTravel->ShouldRecord())
			{
				// First record time and identity of input...
				TimeTravel->AddTimestampedInput(GetWorld()->GetTimeSeconds(), EInputActionEnum::Jump, 1);
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
			TimeTravel->AddTimestampedInput(GetWorld()->GetTimeSeconds(), EInputActionEnum::Stop_Jumping, 0);
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
			TimeTravel->AddTimestampedInput(GetWorld()->GetTimeSeconds(), EInputActionEnum::Move_Forward, Value);
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
			TimeTravel->AddTimestampedInput(GetWorld()->GetTimeSeconds(), EInputActionEnum::Move_Right, Value);
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
			TimeTravel->AddTimestampedInput(GetWorld()->GetTimeSeconds(), EInputActionEnum::Turn, Value);
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
			TimeTravel->AddTimestampedInput(GetWorld()->GetTimeSeconds(), EInputActionEnum::Turn_At_Rate, Rate);
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
			TimeTravel->AddTimestampedInput(GetWorld()->GetTimeSeconds(), EInputActionEnum::Look_Up, Value);
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
			TimeTravel->AddTimestampedInput(GetWorld()->GetTimeSeconds(), EInputActionEnum::Look_Up_At_Rate, Rate);
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

*/

void AChronoCharacter::ReplayPastActions(FTimestampedInputs ActionsToReplay)
{
	auto ThisController = Cast<AChronoPlayerController>(GetController());

	if (ensure(ThisController != nullptr))
	{

		// Loop through the InputValues array of this struct that's past in

		for (int i = 0; i < ActionsToReplay.InputValues.Num(); i++)
		{
			// For every non-zero float value you find:
			if (ActionsToReplay.InputValues[i] != 0.f)
			{
				// Look up the action name in same index in the controller's inputbindings array
				FName WhichAction = ThisController->GetRecordableMovementAndActionBindings()[i];

				if (WhichAction == "Jump")
				{
					Jump(); // This is ACharacter's interface
				}

				else if (WhichAction == "EndJump")
				{
					StopJumping(); // This is ACharacter's interface
				}

				else if (WhichAction == "MoveForward")
				{
					// find out which way is forward
					const FRotator Rotation = Controller->GetControlRotation();
					const FRotator YawRotation(0, Rotation.Yaw, 0);

					// get forward vector
					const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
					AddMovementInput(Direction, ActionsToReplay.InputValues[i]);
				}

				else if (WhichAction == "Moveright")
				{
					// find out which way is forward
					const FRotator Rotation = Controller->GetControlRotation();
					const FRotator YawRotation(0, Rotation.Yaw, 0);

					// get forward vector
					const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
					AddMovementInput(Direction, ActionsToReplay.InputValues[i]);
				}

				else if (WhichAction == "Turn")
				{
					AddControllerYawInput(ActionsToReplay.InputValues[i]);
				}

				else if (WhichAction == "TurnAtRate")
				{
					AddControllerYawInput(ActionsToReplay.InputValues[i] * BaseTurnRate * GetWorld()->GetDeltaSeconds());
				}

				else if (WhichAction == "LookUp")
				{
					AddControllerPitchInput(ActionsToReplay.InputValues[i]);
				}

				else if (WhichAction == "LookUpAtRate")
				{
					AddControllerPitchInput(ActionsToReplay.InputValues[i] * BaseTurnRate * GetWorld()->GetDeltaSeconds());
				}

				else if (WhichAction == "Crouch")
				{
					Crouch(); // This is ACharacter's interface
				}

				else if (WhichAction == "EndCrouch")
				{
					UnCrouch(); // This is ACharacter's interface
				}

				else if (WhichAction == "Grab")
				{
					Grab(); 
				}

				else if (WhichAction == "EndGrab")
				{
					EndGrab(); 
				}

				else if (WhichAction == "HolsterToggle")
				{
					// Toggle the holster / unholster boolean;
					bHolsterButtonDown = bHolsterButtonDown ? false : true;
				}
			}

		}
	}
}

void AChronoCharacter::BeginPlay()
{
	Super::BeginPlay();
	// Configure the weapoon (potentially weapon list)
	
	if (ensure(TimeWeaponClass))
	{
		TimeWeapon = GetWorld()->SpawnActor<ATimeWeapon>(TimeWeaponClass);
		TimeWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), "WeaponHolster");
	}
}
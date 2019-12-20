// Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.


#include "ChronoPlayerController.h"
#include "ChronoCharacter.h"
#include "TimeTravelComponent.h"

AChronoPlayerController::AChronoPlayerController()
{
	// Sets up a time travel component
	TimeTravel = CreateDefaultSubobject<UTimeTravelComponent>(FName("Time Travel Component"));
}

void AChronoPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent);
	SetUpRecordableActionBinding("Jump", IE_Pressed, this, &AChronoPlayerController::Jump); 
	SetUpRecordableActionBinding("Stop Jumping", IE_Released, this, &AChronoPlayerController::StopJumping);

	//SetUpRecordableAxisBinding("MoveForward", this, &AChronoPlayerController::MoveForward);
	//SetUpRecordableAxisBinding("MoveRight", this, &AChronoPlayerController::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick

	//SetUpRecordableAxisBinding("Turn", this, &AChronoPlayerController::Turn);
	//SetUpRecordableAxisBinding("TurnAtRate", this, &AChronoPlayerController::TurnAtRate);
	//SetUpRecordableAxisBinding("LookUp", this, &AChronoPlayerController::LookUp);
	//SetUpRecordableAxisBinding("LookUpAtRate", this, &AChronoPlayerController::LookUpAtRate);

	/* TOUCH DEVICES AND VR HEADSET NOT IMPLEMENTED YET */
}

void AChronoPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

TArray<FName> AChronoPlayerController::GetRecordableMovementAndActionBindings() const
{
	return RecordableMovementAndActionBindings;
}

template <class UserClass>
void AChronoPlayerController::SetUpRecordableActionBinding(const FName NewAction, const EInputEvent KeyEvent, UserClass* Object, typename FInputActionHandlerSignature::TUObjectMethodDelegate< UserClass >::FMethodPtr Func)
{
	// First bind the action we want
	InputComponent->BindAction(NewAction, KeyEvent, Object, Func);
	
	// Keep track of the order in which we add movements/actions in this array
	RecordableMovementAndActionBindings.Add(NewAction);
	
	// Then set it up in the FTimestampedInputs template struct, with a default value for its Value as 0
	// The index of this float in the array will match the index of the action in the
	// RecordableMovementAndActionBindings array above
	
	TimeTravel->TimestampedInputsTemplate.InputValues.Add(0);
	/* NOTE: THE LENGTH OF THESE TWO ARRAYS SHOULD BE IDENTICAL */

	
}

template <class UserClass>
void AChronoPlayerController::SetUpRecordableAxisBinding(const FName NewAction, UserClass* Object, typename FInputAxisHandlerSignature::TUObjectMethodDelegate< UserClass >::FMethodPtr Func)
{
	// First bind the action we want
	InputComponent->BindAxis(NewAction, Object, Func);
	
	// Keep track of the order in which we add movements/actions in this array
	RecordableMovementAndActionBindings.Add(NewAction);
	
	// Then set it up in the FTimestampedInputs struct, with a default value for its Value as 0
	// The index of this float in the array will match the index of the action in the
	// RecordableMovementAndActionBindings array below
	CharacterTTC->TimestampedInputsTemplate.InputValues.Add(0);

	/* NOTE: THE LENGTH OF THE ARRAYS SHOULD BE IDENTICAL */
	
}

void AChronoPlayerController::Jump()
{
	if(MyCharacter)
	{
		GetCharacter()->Jump();

		// Record the action in Character's TimeTravelComponent
		if (ensure(TimeTravel != nullptr))
		{
			if (TimeTravel->ShouldRecord())
			{
				// Record action and pass the array index of that action in the sequnce of bound actions
				// MAKE SURE YOU USE THE SAME NAME TO DESCRIBE THE ACTION
				int32 IndexInFloatArray = RecordableMovementAndActionBindings.Find("Jump");
				TimeTravel->AddTimestampedInput(GetWorld()->GetTimeSeconds(), IndexInFloatArray, 1.f);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s did not have a valid TimeTravelComponent!"), *MyCharacter->GetName())
		}
	}

	
}

void AChronoPlayerController::StopJumping()
{
	GetCharacter()->StopJumping();
}

/*
void AChronoPlayerController::MoveForward(float Value)
{
	if (Value != 0.0f && MyCharacter)
	{
		// find out which way is forward
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		MyCharacter->AddMovementInput(Direction, Value);
	}
}

void AChronoPlayerController::MoveRight(float Value)
{
	if (Value != 0.0f && MyCharacter)
	{
		// find out which way is right
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		MyCharacter->AddMovementInput(Direction, Value);
	}

}

void AChronoPlayerController::Turn(float Value)
{
	if (Value != 0.f && MyCharacter && IsLocalPlayerController())
	{
		AddYawInput(Value);
	}

}

void AChronoPlayerController::TurnAtRate(float Rate)
{
	if (Rate != 0.f && MyCharacter && IsLocalPlayerController())
	{
		//AddYawInput(Rate * MyCharacter->BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void AChronoPlayerController::LookUp(float Value)
{
	if (Value != 0.f && MyCharacter && IsLocalPlayerController())
	{
		AddPitchInput(Value);
	}
}

void AChronoPlayerController::LookUpAtRate(float Rate)
{
	if (Rate != 0.f && MyCharacter && IsLocalPlayerController())
	{
		//AddPitchInput(Rate * MyCharacter->BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

*/
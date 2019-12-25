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
	SetUpRecordableActionBinding("Jump", IE_Released, this, &AChronoPlayerController::EndJump);

	SetUpRecordableAxisBinding("MoveForward", this, &AChronoPlayerController::MoveForward);
	SetUpRecordableAxisBinding("MoveRight", this, &AChronoPlayerController::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	SetUpRecordableAxisBinding("Turn", this, &AChronoPlayerController::Turn);
	SetUpRecordableAxisBinding("TurnAtRate", this, &AChronoPlayerController::TurnAtRate);
	SetUpRecordableAxisBinding("LookUp", this, &AChronoPlayerController::LookUp);
	SetUpRecordableAxisBinding("LookUpAtRate", this, &AChronoPlayerController::LookUpAtRate);

	SetUpRecordableActionBinding("Crouch", IE_Pressed, this, &AChronoPlayerController::Crouch);
	SetUpRecordableActionBinding("Crouch", IE_Released, this, &AChronoPlayerController::EndCrouch);

	SetUpRecordableActionBinding("Grab", IE_Pressed, this, &AChronoPlayerController::Grab);
	SetUpRecordableActionBinding("Grab", IE_Released, this, &AChronoPlayerController::EndGrab);

	SetUpRecordableActionBinding("HolsterToggle", IE_Pressed, this, &AChronoPlayerController::HolsterToggle);

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
	// Deal with the special case that this is a "key released" type of actions; presumably we have bound
	// the same action for "key pressed"
	if (KeyEvent == IE_Released)
	{
		FString NewActionString = "End" + NewAction.ToString();
		FName EndNewAction(*NewActionString);
		RecordableMovementAndActionBindings.Add(EndNewAction);
	}
	else
	{
		RecordableMovementAndActionBindings.Add(NewAction);
	}

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
	TimeTravel->TimestampedInputsTemplate.InputValues.Add(0);

	/* NOTE: THE LENGTH OF THE ARRAYS SHOULD BE IDENTICAL */
	
}

void AChronoPlayerController::RecordAction(FName ActionToRecord, float Value)
{
	// Record the action in Character's TimeTravelComponent
	if (ensure(TimeTravel != nullptr))
	{

		if (TimeTravel->ShouldRecord())
		{

			// Record action and pass the array index of that action in the sequnce of bound actions
			// MAKE SURE YOU USE THE SAME NAME TO DESCRIBE THE ACTION
			int32 IndexInFloatArray = RecordableMovementAndActionBindings.Find(ActionToRecord);
			//UE_LOG(LogTemp, Warning, TEXT("So far so good, int index of %i"), IndexInFloatArray)
			TimeTravel->AddTimestampedInput(GetWorld()->GetTimeSeconds(), IndexInFloatArray, Value);
			
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s did not have a valid TimeTravelComponent!"), *GetCharacter()->GetName())
	}
}

void AChronoPlayerController::Jump()
{
	if (ensure(GetCharacter() != nullptr))
	{
		GetCharacter()->Jump();
		RecordAction("Jump", 1.f);
	}	
}

void AChronoPlayerController::EndJump()
{
	if (ensure(GetCharacter() != nullptr))
	{
		GetCharacter()->StopJumping();
		RecordAction("EndJump", 1.f);
	}
}


void AChronoPlayerController::MoveForward(float Value)
{
	if (ensure(GetCharacter() != nullptr))
	{
		// find out which way is forward
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		GetCharacter()->AddMovementInput(Direction, Value);
		RecordAction("MoveForward", Value);
	}

}


void AChronoPlayerController::MoveRight(float Value)
{
	if (ensure(GetCharacter() != nullptr))
	{
		// find out which way is forward
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		GetCharacter()->AddMovementInput(Direction, Value);
		RecordAction("MoveRight", Value);
	}

}


void AChronoPlayerController::Turn(float Value)
{
	if (GetCharacter() && IsLocalPlayerController())
	{
		AddYawInput(Value);
		RecordAction("Turn", Value);
	}

}

void AChronoPlayerController::TurnAtRate(float Rate)
{
	if (GetCharacter() && IsLocalPlayerController())
	{
		AddYawInput(Rate * Cast<AChronoCharacter>(GetCharacter())->BaseTurnRate * GetWorld()->GetDeltaSeconds());
		RecordAction("TurnAtRate", Rate);
	}
}

void AChronoPlayerController::LookUp(float Value)
{
	if (GetCharacter() && IsLocalPlayerController())
	{
		AddPitchInput(Value);
		RecordAction("LookUp", Value);
	}
}

void AChronoPlayerController::LookUpAtRate(float Rate)
{
	if (GetCharacter() && IsLocalPlayerController())
	{
		AddPitchInput(Rate * Cast<AChronoCharacter>(GetCharacter())->BaseLookUpRate * GetWorld()->GetDeltaSeconds());
		RecordAction("LookUpAtRate", Rate);
	}
}

void AChronoPlayerController::Crouch()
{
	if (ensure(GetCharacter() != nullptr))
	{
		GetCharacter()->Crouch();
		RecordAction("Crouch", 1.f);
	}
}

void AChronoPlayerController::EndCrouch()
{
	if (ensure(GetCharacter() != nullptr))
	{
		GetCharacter()->UnCrouch();
		RecordAction("EndCrouch", 1.f);
	}
}

void AChronoPlayerController::Grab()
{
	if (ensure(GetCharacter() != nullptr))
	{
		RecordAction("Grab", 1.f);
		CastChecked<AChronoCharacter>(GetCharacter())->Grab();
	}
}

void AChronoPlayerController::EndGrab()
{
	if (ensure(GetCharacter() != nullptr))
	{
		RecordAction("EndGrab", 1.f);
		CastChecked<AChronoCharacter>(GetCharacter())->EndGrab();	
	}
}

void AChronoPlayerController::HolsterToggle()
{
	if (ensure(GetCharacter() != nullptr))
	{
		RecordAction("HolsterToggle", 1.f);
	}
	// ...
}
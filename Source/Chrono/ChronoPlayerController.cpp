// Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.


#include "ChronoPlayerController.h"
#include "ChronoCharacter.h"
#include "TimeTravelComponent.h"
#include "ChronoGameInstance.h"
#include "InteractionComponent.h"

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

	SetUpRecordableActionBinding("Interact", IE_Pressed, this, &AChronoPlayerController::Interact);
	SetUpRecordableActionBinding("DropObject", IE_Pressed, this, &AChronoPlayerController::DropObject);

	SetUpRecordableActionBinding("HolsterToggle", IE_Pressed, this, &AChronoPlayerController::HolsterToggle);

	SetUpRecordableActionBinding("Aim", IE_Pressed, this, &AChronoPlayerController::AimToggle);
	SetUpRecordableActionBinding("Aim", IE_Released, this, &AChronoPlayerController::AimToggle);

	SetUpRecordableActionBinding("Fire", IE_Pressed, this, &AChronoPlayerController::Fire);

	/* TOUCH DEVICES AND VR HEADSET NOT IMPLEMENTED YET */
}

void AChronoPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AChronoPlayerController::OnPossess(APawn* ControlledPawn)
{
	Super::OnPossess(ControlledPawn);

	MyChronoCharacter = CastChecked<AChronoCharacter>(ControlledPawn);
	if (MyChronoCharacter)
	{
		MyChronoCharacter->SetMyChronoController(this);
	}
}

AChronoCharacter* AChronoPlayerController::GetMyChronoCharacter() const
{
	return MyChronoCharacter;
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
}

template <class UserClass>
void AChronoPlayerController::SetUpRecordableAxisBinding(const FName NewAction, UserClass* Object, typename FInputAxisHandlerSignature::TUObjectMethodDelegate< UserClass >::FMethodPtr Func)
{
	// First bind the action we want
	InputComponent->BindAxis(NewAction, Object, Func);
	
	// Keep track of the order in which we add movements/actions in this array
	RecordableMovementAndActionBindings.Add(NewAction);
}

void AChronoPlayerController::RecordAction(FName ActionToRecord, float Value)
{
	// Record the action in Character's TimeTravelComponent
	if (ensure(TimeTravel != nullptr))
	{

		if (TimeTravel->ShouldRecord())
		{

			// Record action and its associated value
			// MAKE SURE YOU USE THE SAME NAME TO DESCRIBE THE ACTION
			FRecordedActionInput NewRecordedActionInput;
			NewRecordedActionInput.RecordedAction = ActionToRecord;
			NewRecordedActionInput.InputValue = Value;

			TimeTravel->AddTimestampedInput(GetWorld()->GetTimeSeconds(), NewRecordedActionInput);
			
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s did not have a valid TimeTravelComponent!"), *GetCharacter()->GetName())
	}
}

void AChronoPlayerController::Jump()
{
	if (GetCharacter())
	{
		GetCharacter()->Jump();
		RecordAction("Jump", 1.f);
	}	
}

void AChronoPlayerController::EndJump()
{
	if (GetCharacter())
	{
		GetCharacter()->StopJumping();
		RecordAction("EndJump", 1.f);
	}
}


void AChronoPlayerController::MoveForward(float Value)
{
	if (GetCharacter())
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
	if (GetCharacter())
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
	if (GetCharacter()) 
	{
		AddYawInput(Value);
		RecordAction("Turn", Value);
	}

}

void AChronoPlayerController::TurnAtRate(float Rate)
{
	if (GetCharacter())
	{
		AddYawInput(Rate * MyChronoCharacter->BaseTurnRate * GetWorld()->GetDeltaSeconds());
		RecordAction("TurnAtRate", Rate);
	}
}

void AChronoPlayerController::LookUp(float Value)
{
	if (GetCharacter())
	{
		AddPitchInput(Value);
		RecordAction("LookUp", Value);
	}
}

void AChronoPlayerController::LookUpAtRate(float Rate)
{
	if (GetCharacter())
	{
		AddPitchInput(Rate *MyChronoCharacter->BaseLookUpRate * GetWorld()->GetDeltaSeconds());
		RecordAction("LookUpAtRate", Rate);
	}
}

void AChronoPlayerController::Crouch()
{
	if (GetCharacter())
	{
		GetCharacter()->Crouch();
		RecordAction("Crouch", 1.f);
	}
}

void AChronoPlayerController::EndCrouch()
{
	if (GetCharacter())
	{
		GetCharacter()->UnCrouch();
		RecordAction("EndCrouch", 1.f);
	}
}

void AChronoPlayerController::DropObject()
{
	if (GetCharacter())
	{
		RecordAction("DropObject", 1.f);
		
		// Check to see if we are actually grabbing something first
		//if (MyChronoCharacter->GetGrabButtonDown())
		//{
			// Already holding something, so drop it!
			//MyChronoCharacter->SetGrabButtonDown(false);
			MyChronoCharacter->EndGrab();
		//}
	}
}

void AChronoPlayerController::Interact()
{
	if (GetCharacter())
	{
		//if (UInteractionComponent* InteractionToExecute = Cast<UChronoGameInstance>(GetGameInstance())->GetCurrentInteractionCommand())
		//Alternative version of the above (with array of UInteractionComponent*) follows:
		if (UInteractionComponent* InteractionToExecute = Cast<UChronoGameInstance>(GetGameInstance())->GetLatestInteractionCommand())
		{
			InteractionToExecute->ExecuteInteraction(MyChronoCharacter);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Special Interaction Command Found... Reverting to Default!"))
		}
	}
}

void AChronoPlayerController::HolsterToggle()
{
	if (GetCharacter())
	{
		RecordAction("HolsterToggle", 1.f);
		
		// Toggle between holstering/unholstering a weapon (meant to be able to interrupt animations mid-way through)
		if (MyChronoCharacter->GetHolsterButtonDown())
		{
			MyChronoCharacter->SetHolsterButtonDown(false);
		}
		else
		{
			MyChronoCharacter->SetHolsterButtonDown(true);
		}
	}
}

void AChronoPlayerController::AimToggle() // TODO Vaggelis: I have to check for equiped weapon first!
{
	if (GetCharacter())
	{
		if(MyChronoCharacter->GetHasEquippedWeapon())
		{
			RecordAction("Aim", 1.f);
			MyChronoCharacter->ToggleCameras();
			MyChronoCharacter->ToggleAimButtonDown();
		}
	}
}

void AChronoPlayerController::Fire()
{
	if (GetCharacter())
	{
		if (MyChronoCharacter->GetHasEquippedWeapon())
		{
			RecordAction("Fire", 1.f);
			MyChronoCharacter->Fire();
		}
	}
}

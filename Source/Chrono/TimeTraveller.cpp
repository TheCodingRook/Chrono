// A prototype by Evangelos ("Vaggelis") Tsesmelidakis. All rights reserved. 2019


#include "TimeTraveller.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "Engine/Engine.h" // TODO: Vaggelis: for debug purposes, consider removing at the end
#include "TimerManager.h"

ATimeTraveller::ATimeTraveller()
{
	//Super(); TODO: Do I need to call Super?

	TimeTravel = CreateDefaultSubobject<UTimeTravelComponent>(FName("Time Travel Component"));
}

UTimeTravelComponent* ATimeTraveller::GetTimeTravelComponent() const
{
	return TimeTravel;
}

void ATimeTraveller::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATimeTraveller::JumpAndRecord);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ATimeTraveller::StopJumpingAndRecord);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATimeTraveller::MoveForwardAndRecord);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATimeTraveller::MoveRightAndRecord);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick

	PlayerInputComponent->BindAxis("Turn", this, &ATimeTraveller::TurnAndRecord);
	PlayerInputComponent->BindAxis("TurnAtRate", this, &ATimeTraveller::TurnAtRateAndRecord);
	PlayerInputComponent->BindAxis("LookUp", this, &ATimeTraveller::LookUpAndRecord);
	PlayerInputComponent->BindAxis("LookUpAtRate", this, &ATimeTraveller::LookUpAtRateAndRecord);

	
	// TODO: Vaggelis: Implement recorded input actions for touch and VR devices
	// handle touch devices
	//PlayerInputComponent->BindTouch(IE_Pressed, this, &AChronoCharacter::TouchStarted);
	//PlayerInputComponent->BindTouch(IE_Released, this, &AChronoCharacter::TouchStopped);

	// VR headset functionality
	//PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AChronoCharacter::OnResetVR);
}

void ATimeTraveller::JumpAndRecord()
{
	if (TimeTravel->ShouldRecord())
	{ 
		// First record time and identity of input...
		TimeTravel->AddRecordedAction(GetWorld()->GetTimeSeconds(), EInputActionEnum::Jump, 0);

		// ... then execute action
		//Jump();
	}
}

void ATimeTraveller::StopJumpingAndRecord()
{
	if (TimeTravel->ShouldRecord())
	{
		// First record time and identity of input...
		TimeTravel->AddRecordedAction(GetWorld()->GetTimeSeconds(), EInputActionEnum::Stop_Jumping, 0);

		// ... then execute action
		//StopJumping();
	}
}

void ATimeTraveller::MoveForwardAndRecord(float Value)
{
	if (TimeTravel->ShouldRecord())
	{
		// First record time and identity of input...
		TimeTravel->AddRecordedAction(GetWorld()->GetTimeSeconds(), EInputActionEnum::Move_Forward, Value);

		// ... then execute action
		MoveForward(Value);
	}
}

void ATimeTraveller::MoveRightAndRecord(float Value)
{
	if (TimeTravel->ShouldRecord())
	{ 
		// First record time and identity of input...
		TimeTravel->AddRecordedAction(GetWorld()->GetTimeSeconds(), EInputActionEnum::Move_Right, Value);

		// ... then execute action
		MoveRight(Value);
	}
}

void ATimeTraveller::TurnAndRecord(float Value)
{
	if (TimeTravel->ShouldRecord())
	{
		// First record time and identity of input...
		TimeTravel->AddRecordedAction(GetWorld()->GetTimeSeconds(), EInputActionEnum::Turn, Value);

		// ... then execute action
		AddControllerYawInput(Value);
	}
}

void ATimeTraveller::TurnAtRateAndRecord(float Value)
{
	if (TimeTravel->ShouldRecord())
	{
		// First record time and identity of input...
		TimeTravel->AddRecordedAction(GetWorld()->GetTimeSeconds(), EInputActionEnum::Turn_At_Rate, Value);

		// ... then execute action
		TurnAtRate(Value);
	}
}

void ATimeTraveller::LookUpAndRecord(float Value)
{
	if (TimeTravel->ShouldRecord())
	{
		// First record time and identity of input...
		TimeTravel->AddRecordedAction(GetWorld()->GetTimeSeconds(), EInputActionEnum::Look_Up, Value);

		// ... then execute action
		AddControllerPitchInput(Value);
	}
}

void ATimeTraveller::LookUpAtRateAndRecord(float Value)
{
	if (TimeTravel->ShouldRecord())
	{
		// First record time and identity of input...
		TimeTravel->AddRecordedAction(GetWorld()->GetTimeSeconds(), EInputActionEnum::Look_Up_At_Rate, Value);

		// ... then execute action
		LookUpAtRate(Value);
	}
}

void ATimeTraveller::FireAndRecord()
{
	if (TimeTravel->ShouldRecord())
	{
		// First record time and identity of input...
		TimeTravel->AddRecordedAction(GetWorld()->GetTimeSeconds(), EInputActionEnum::Fire, 0);

		// ... then execute action
		//Fire();
	}
}

void ATimeTraveller::ReplayHistory()
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
			GetWorldTimerManager().SetTimer(Delay, this, &ATimeTraveller::ReplayAction, 0.001f, false);
		}
		else
		{
			GetWorldTimerManager().SetTimer(Delay, this, &ATimeTraveller::ReplayAction, TimeTravel->GetPastActions()[i].TimeStamp - TimeTravel->GetPastActions()[i-1].TimeStamp, false);
		}
		i++;
	}
}

void ATimeTraveller::ReplayAction()
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
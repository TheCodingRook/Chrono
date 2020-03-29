// Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.


#include "PastSelfController.h"
#include "ChronoCharacter.h"
#include "ChronoPlayerController.h"
#include "TimeWeapon.h"

AChronoCharacter* APastSelfController::GetMyChronoCharacter() const
{
	return MyChronoCharacter;
}

void APastSelfController::OnPossess(APawn* ControlledPawn)
{
	Super::OnPossess(ControlledPawn);

	MyChronoCharacter = CastChecked<AChronoCharacter>(ControlledPawn);
	if (MyChronoCharacter)
	{
		MyChronoCharacter->SetMyPastSelfController(this);
	}
}

void APastSelfController::ReplayPastActions(const FTimestampedActions& ActionsToReplay)
{	
	// Instigator reference
	AChronoCharacter* InstigatorCharacter = CastChecked<AChronoCharacter>(GetInstigator());

	if (InstigatorCharacter->GetMyChronoController())
	{
		// Loop through the InputValues array of this struct that was passed in
		for (int i = 0; i < ActionsToReplay.RecordedActionInputArray.Num(); i++)
		{
			// For every non-zero float value you find:
			//if (ActionsToReplay.RecordedActionInputArray[i].InputValue != 0.f)
			//{
				// Look up the action name in same index in the controller's inputbindings array
				FName WhichAction = ActionsToReplay.RecordedActionInputArray[i].RecordedAction;
				if (WhichAction == NAME_None)
				{
					break;
				}

				else if (WhichAction == "Jump")
				{
					MyChronoCharacter->Jump(); // This is ACharacter's interface
				}

				else if (WhichAction == "EndJump")
				{
					MyChronoCharacter->StopJumping(); // This is ACharacter's interface
				}

				else if (WhichAction == "MoveForward")
				{
					// find out which way is forward
					const FRotator Rotation = GetControlRotation();
					const FRotator YawRotation(0, Rotation.Yaw, 0);

					// get forward vector
					const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
					MyChronoCharacter->AddMovementInput(Direction, ActionsToReplay.RecordedActionInputArray[i].InputValue);
				}

				else if (WhichAction == "Moveright")
				{
					// find out which way is forward
					const FRotator Rotation = GetControlRotation();
					const FRotator YawRotation(0, Rotation.Yaw, 0);

					// get forward vector
					const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
					MyChronoCharacter->AddMovementInput(Direction, ActionsToReplay.RecordedActionInputArray[i].InputValue);
				}

				else if (WhichAction == "Turn")
				{
					MyChronoCharacter->AddControllerYawInput(ActionsToReplay.RecordedActionInputArray[i].InputValue);
				}

				else if (WhichAction == "TurnAtRate")
				{
					MyChronoCharacter->AddControllerYawInput(ActionsToReplay.RecordedActionInputArray[i].InputValue * MyChronoCharacter->BaseTurnRate * GetWorld()->GetDeltaSeconds());
				}

				else if (WhichAction == "LookUp")
				{
					MyChronoCharacter->AddControllerPitchInput(ActionsToReplay.RecordedActionInputArray[i].InputValue);
				}

				else if (WhichAction == "LookUpAtRate")
				{
					MyChronoCharacter->AddControllerPitchInput(ActionsToReplay.RecordedActionInputArray[i].InputValue * MyChronoCharacter->BaseTurnRate * GetWorld()->GetDeltaSeconds());
				}

				else if (WhichAction == "Crouch")
				{
					MyChronoCharacter->Crouch(); // This is ACharacter's interface
				}

				else if (WhichAction == "EndCrouch")
				{
					MyChronoCharacter->UnCrouch(); // This is ACharacter's interface
				}

				else if (WhichAction == "DropObject")
				{
					// Make sure first we are indeed holding something 
					if (MyChronoCharacter->GetGrabButtonDown())
					{
						// Already holding something, so drop it!
						MyChronoCharacter->SetGrabButtonDown(false);
						MyChronoCharacter->EndGrab();
					}
				}

				else if (WhichAction == "HolsterToggle")
				{
					// Toggle the holster / unholster boolean
					MyChronoCharacter->SetHolsterButtonDown(MyChronoCharacter->GetHolsterButtonDown() ? false : true);
				}

				else if (WhichAction == "Aim")
				{
					// Toggle AimButtonDown 
					MyChronoCharacter->ToggleAimButtonDown();

					// Toggle the rotation properties of the character (APawn interface) for this "FPS-style" view (no need to worry about camera for past self...)
					MyChronoCharacter->bUseControllerRotationYaw = !MyChronoCharacter->bUseControllerRotationYaw;
				}

				else if (WhichAction == "Fire")
				{
					if (MyChronoCharacter->GetTimeWeapon())
					{
						MyChronoCharacter->GetTimeWeapon()->Fire();
					}
				}
				
			//}
		}
		// TODO Vaggelis: I don't know why this works... not sure why I have to call this explicitly but it works fairly accurately (BUT NOT ALWAYS!)
		// finally updating the rotation of the character when replaying history of "turn" and "lookup" type rotations
		// Though it DOES NOT WORK for the "spawn second player in game" approach!
		UpdateRotation(0.f);
	}
	
}
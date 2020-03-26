// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ChronoCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ChronoPlayerController.h"
#include "PastSelfController.h"
#include "GameFramework/SpringArmComponent.h"
#include "GrabbingAbility.h"
#include "TimeWeapon.h"
#include "Components\ArrowComponent.h"


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
	CameraBoom->SetRelativeLocation(FVector(0.f, 30.f, 85.f));
	CameraBoom->TargetArmLength = 350.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create an arrow component to indicate aim camera location
	AimCameraLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("AimCameraLocation"));
	AimCameraLocation->SetupAttachment(CameraBoom);
	AimCameraLocation->SetRelativeLocation(FVector(0.f, 5.f, -20.f)); // Set the aim camera next to the head for an "over-the-shoulder" view point effect

	// Initialize the holstertoggle and aimtoggle booleans
	bHolsterButtonDown = false;
	bAimButtonDown = false;

	// Set the camera offset before toggle begins
	CameraOffset = AimCameraLocation->GetRelativeLocation();
	BoomOffset = -CameraBoom->TargetArmLength;

	// Set a default value for Health
	Health = 100;
}

AChronoPlayerController* AChronoCharacter::GetMyChronoController() const
{
	return MyChronoController;
}

APastSelfController* AChronoCharacter::GetMyPastSelfController() const
{
	return MyPastSelfController;
}

void AChronoCharacter::SetMyChronoController(AChronoPlayerController* ControllerToSet)
{
	MyChronoController = ControllerToSet;
}

void AChronoCharacter::SetMyPastSelfController(APastSelfController* ControllerToSet)
{
	MyPastSelfController = ControllerToSet;
}

ATimeWeapon* AChronoCharacter::GetTimeWeapon() const
{
	return TimeWeapon;
}

void AChronoCharacter::Grab()
{
	UGrabbingAbility* Grabber = FindComponentByClass<UGrabbingAbility>();
	if (Grabber != nullptr)
	{
		//Grabber->GrabObject();
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
	bHasEquippedWeapon = true;
}

void AChronoCharacter::UnEquipWeapon()
{
	TimeWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), "WeaponHolster");
	bHasEquippedWeapon = false;
}

void AChronoCharacter::ToggleCameras()
{
	FollowCamera->AddRelativeLocation(CameraOffset);
	// Set the camera offset as the negative of the previous one so it toggles back and forth into place
	CameraOffset = -CameraOffset;

	// Do similar adjustment for the camera boom; we want this to be zero as it is supposed to be attached over character's shoulder
	CameraBoom->TargetArmLength += BoomOffset;
	BoomOffset = -BoomOffset;

	// Toggle the rotation properties of the character (APawn interface) for this "FPS-style" view
	bUseControllerRotationYaw = !bUseControllerRotationYaw;
}

void AChronoCharacter::ToggleAimButtonDown()
{
	bAimButtonDown = bAimButtonDown ? false : true;
}

bool AChronoCharacter::GetAimButtonDown() const
{
	return bAimButtonDown;
}

void AChronoCharacter::SetAimButtonDown(bool InFlag)
{
	bAimButtonDown = InFlag;
}

bool AChronoCharacter::GetHasEquippedWeapon() const
{
	return bHasEquippedWeapon;
}

void AChronoCharacter::Fire()
{
	if (TimeWeapon)
	{
		TimeWeapon->Fire();
	}
}

bool AChronoCharacter::GetGrabButtonDown() const
{
	return bGrabButtonDown;
}

void AChronoCharacter::SetGrabButtonDown(bool InFlag)
{
	bGrabButtonDown = InFlag;
}

float AChronoCharacter::GetHealth() const
{
	return Health;
}

void AChronoCharacter::SetHealth(float NewHealthAmount)
{
	Health = NewHealthAmount;
}

void AChronoCharacter::ReduceHealth(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	// We probably don't care if this gets getting too negative.
	SetHealth(Health - Damage);

	if (Health < 0 )
	{
		KillCharacter();
	}
}

void AChronoCharacter::KillCharacter()
{
	if (GetController()) // Check for controller because if the character has died already, it will been unpossessed as well.
	{
		// Depossess the character and simulate physics to simulate the character collapsing
		//GetController()->UnPossess(); // Depossessing uncovered a bug if the past self is still executing recorded actions/movements!!!
		// TODO Vaggelis: Figure out how to deal with death of character and if unpossessing is necessary then how to do it cleanly.
		GetMesh()->BodyInstance.SetCollisionProfileName("BlockAll"); // to make sure dead body doesn't go through the floor/ground/surface it's on.
		GetMesh()->SetSimulatePhysics(true);
	}
}

void AChronoCharacter::OnTravelBackInTime_Implementation()
{
	//blank on purpose
}

void AChronoCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Bind the delegate for projectile impacts
	OnTakeAnyDamage.AddDynamic(this, &AChronoCharacter::ReduceHealth);

	// Configure the weapoon (potentially weapon list)
	if (ensure(TimeWeaponClass))
	{
		// Set up some spawn parameters for future use, for example when implementing damage events
		FActorSpawnParameters WeaponSpawnParams;
		WeaponSpawnParams.Owner = this;
		WeaponSpawnParams.Instigator = this;

		TimeWeapon = GetWorld()->SpawnActor<ATimeWeapon>(TimeWeaponClass, WeaponSpawnParams);
		TimeWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), "WeaponHolster");
	}
}
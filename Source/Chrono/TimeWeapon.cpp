// A prototype by Evangelos ("Vaggelis") Tsesmelidakis. All rights reserved. 2019


#include "TimeWeapon.h"
#include "Components/WidgetComponent.h"
#include "Projectile.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ATimeWeapon::ATimeWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	RootComponent = GunMesh;
	
	AmmoWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("AmmoWidget"));
	AmmoWidget->SetupAttachment(GunMesh);
	
	// Set the default energy related values here
	EnergyCharge = 100;
	EnergyBlast = 10;

}

// Called when the game starts or when spawned
void ATimeWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATimeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ATimeWeapon::GetEnergyCharge() const
{
	return EnergyCharge;
}

void ATimeWeapon::SetEnergyCharge(float NewCharge)
{ 
	EnergyCharge = NewCharge;
}

float ATimeWeapon::GetEnergyBlast() const
{
	return EnergyBlast;
}

/** Base implementation of Fire, but can be overriden/extended in Blueprint*/
void ATimeWeapon::Fire_Implementation()
{
	if (EnergyCharge >= EnergyBlast)
	{
		EnergyCharge = FMath::Clamp<float>(EnergyCharge - EnergyBlast, 0 , EnergyCharge);

		FVector SpawnLocation = GunMesh->GetSocketLocation("Muzzle");
		FRotator SpawnRotation = GunMesh->GetSocketRotation("Muzzle");

		/** Make sure  there is a Projectile class selected and if so fire weapon*/
		if (ProjectileClass != nullptr)
		{
			auto NewProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
		}
		
		
	}
}

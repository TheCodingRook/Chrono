// A prototype by Evangelos ("Vaggelis") Tsesmelidakis. All rights reserved. 2019


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ChronoCharacter.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Damage = 0;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	// Leave the rest of the implentation for the derived classes 
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	

}
/*
void AProjectile::OnHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	//Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	UE_LOG(LogTemp, Warning, TEXT("Applying damage"))
	// Apply damage to other actor, whoever they are. Let them deal with the effects of that (if any)
	FPointDamageEvent ProjectileDamageEvent;
	float ActualDamage = Other->TakeDamage(Damage, ProjectileDamageEvent, GetInstigator()->GetController(), this);
	UE_LOG(LogTemp, Warning, TEXT("Applying damage of: %f"), ActualDamage)
}
*/

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UProjectileMovementComponent* AProjectile::GetProjectileMovement() const
{
	return ProjectileMovement;
}

float AProjectile::GetDamage() const
{
	return Damage;
}


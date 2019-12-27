// Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.


#include "BulletProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ABulletProjectile::ABulletProjectile()
{
	CollisionShape = CreateDefaultSubobject<USphereComponent>("Collision Component");
	CollisionShape->InitSphereRadius(5.0f);
	CollisionShape->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionShape->OnComponentHit.AddDynamic(this, &ABulletProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionShape->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionShape->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionShape;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	
	ProjectileMovement->UpdatedComponent = CollisionShape;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	// Setup this projectile's Damage value
	Damage = 10;


}

void ABulletProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	FPointDamageEvent ProjectileDamageEvent;
	float ActualDamage = OtherActor->TakeDamage(Damage, ProjectileDamageEvent, GetInstigator()->GetController(), this);
}

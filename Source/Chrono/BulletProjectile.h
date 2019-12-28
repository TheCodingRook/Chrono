// Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "BulletProjectile.generated.h"

/**
 * Class defining a bullet projectile
 */
UCLASS()
class CHRONO_API ABulletProjectile : public AProjectile
{
	GENERATED_BODY()

	ABulletProjectile();

	UPROPERTY(VisibleDefaultsOnly, Category = "Bullet")
	class USphereComponent* CollisionShape;

	UPROPERTY(EditDefaultsOnly, Category = "Bullet")
	class UStaticMeshComponent* Bullet;
	
public:
	/** called when projectile hits something */
	UFUNCTION(Category = "Event Dispatchers")
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionShape subobject **/
	UFUNCTION(BlueprintCallable)
	USphereComponent* GetCollisionComp() const;
};

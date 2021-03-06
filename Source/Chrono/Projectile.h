// Copyright 2020. All rights reserved. A prototype by CodingRook.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

/* Base class for all projectiles in the game */
UCLASS()
class CHRONO_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Returns ProjectileMovement subobject **/
	class UProjectileMovementComponent* GetProjectileMovement() const;

	//Getter for damage dealt by the projectile, if any
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Damage")
	float GetDamage() const;




protected:
	/* Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float Damage;

	
};

// Copyright 2020. All rights reserved. A prototype by CodingRook.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimeWeapon.generated.h"

// Forward declarations
class USkeletalMeshComponent;
class UWidgetComponent;
class AProjectile;
class USoundBase;

UCLASS()
class CHRONO_API ATimeWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimeWeapon();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Return the energy level of the weapon
	UFUNCTION(BlueprintPure, Category = "Firing")
	float GetEnergyCharge() const;
	
	// Set the energy charge for the weapon
	UFUNCTION(BlueprintCallable, Category = "Firing")
	void SetEnergyCharge (float NewCharge);
	
	// Return the enery cost for firing
	UFUNCTION(BlueprintPure, Category = "Firing")
	float GetEnergyBlast() const;
	
	// Fire the weapon
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Firing")
	void Fire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Stores how much energy charge the weapon is holding
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float EnergyCharge;

	// How much energy does the projectile (blast) carry?
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float EnergyBlast;
	
private:
	/** Set up the mesh and the ammo counter for the weapon*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* GunMesh; // forward decleration here
	
	// Setup the ammo display on the weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* AmmoWidget;
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AProjectile> ProjectileClass;

	// Reference to the sound asset to play when firing the weapon
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	USoundBase* FireSound;
};

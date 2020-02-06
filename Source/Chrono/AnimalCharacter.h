// Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AnimalCharacter.generated.h"

UCLASS()
class CHRONO_API AAnimalCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAnimalCharacter();

	// Differentiate between a walking max speed and running/galloping max speed
	// Max walk speed will initially be driven by CharacterMovementComponent's setting!!!
	UFUNCTION(BlueprintPure, Category = "Animal Movement")
	FORCEINLINE float GetMaxAnimalWalkSpeed() const { return MaxAnimalWalkSpeed; }
	UFUNCTION(BlueprintPure, Category = "Animal Movement")
	FORCEINLINE float GetMaxAnimalRunSpeed() const { return MaxAnimalRunSpeed; }
	UFUNCTION(BlueprintPure, Category = "Animal Behavior")
	FORCEINLINE float GetMinSafetyDistance() const { return MinSafetyDistance; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Animal Movement")
	float MaxAnimalWalkSpeed; // To be set by CharacterMovementComponent's max walk speed value initially
	UPROPERTY(EditDefaultsOnly, Category = "Animal Movement")
	float MaxAnimalRunSpeed;
	UPROPERTY(EditDefaultsOnly, Category = "Animal Behavior")
	float MinSafetyDistance;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};

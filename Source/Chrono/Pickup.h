// Copyright 2020. All rights reserved. A prototype by CodingRook.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

/**
 * Base class to implement consumable pickup items in the world
 */

UCLASS()
class CHRONO_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintPure, Category = "Pickup")
	UStaticMeshComponent* GetMesh() const;
	
	UFUNCTION(BlueprintPure, Category = "Pickup")
	bool isActive();
	
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void SetActive(bool NewPickupState);
	
	// Called when the pickup is collected
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Pickup")
	void WasCollected();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	bool bIsActive;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PickupMesh; // forward decleration here

};

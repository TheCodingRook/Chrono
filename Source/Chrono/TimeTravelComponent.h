// A prototype by Evangelos ("Vaggelis") Tsesmelidakis. All rights reserved. 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimeTravelComponent.generated.h"

/*
*	Simple struct to store a specific input action and the exact time it was received
*/

UENUM(BlueprintType)
enum class EInputActionEnum : uint8
{
	Move_Forward UMETA(DisplayName = "Move Forward"),
	Move_Right UMETA(DisplayName = "Move Right"),
	Turn UMETA(DisplayName = "Turn"),
	Turn_At_Rate UMETA(DisplayName = "Turn At Rate"),
	Look_Up UMETA(DisplayName = "Look Up"),
	Look_Up_At_Rate UMETA(DisplayName = "Look Up At Rate"),
	Jump UMETA(DisplayName = "Jump"),
	Stop_Jumping UMETA(DisplayName = "Stop Jumping"),
	Fire UMETA(DisplayName = "Fire")
};

USTRUCT(BlueprintType)
struct FRecordedInputAction
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Struct Contents")
	float TimeStamp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Struct Contents")
	EInputActionEnum ActionName;

	// Stores the value for axis-related actions, can be set to zero for button actions
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Struct Contents")
	float Value;
	// TODO Vaggelis: designated the members of the struct as BlueprintReadWrite so I can make sure I can see them in BP, however, should they be writeable????
};


/*	A class that allows the actor (in this case most likely the controlled pawn) to
* 	store its controller's input so it can be re-played at a later time. Intended to
*	be used to simulate a Player travelling back in time and encountering "past self".
*/	
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHRONO_API UTimeTravelComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTimeTravelComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	/* Public getter functions */
	UFUNCTION(BlueprintPure, Category = "Time Travel")
	TArray<FRecordedInputAction> GetPastActions() const;

	UFUNCTION(BlueprintPure, Category = "Time Travel")
	float GetMaxRecordingTime() const;

	UFUNCTION(BlueprintPure, Category = "Time Travel")
	int GetMaxStructArraySize() const;

	UFUNCTION(BlueprintPure, Category = "Time Travel")
	bool ShouldRecord() const;

	/*Add to the array*/
	UFUNCTION(BlueprintCallable, Category = "Time Travel")
	void AddRecordedAction(float TimeStamp, EInputActionEnum RecordedActionName, float RecordedValue);

	UFUNCTION(BlueprintCallable, Category = "Time Travel")
	void AllowRecording(bool bInCanRecord);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
private:
	// Array of structs to record a continuous stream of input actions
	UPROPERTY(VisibleAnywhere, Category = "Time Travel")
	TArray<FRecordedInputAction> PastActions;

	// Maximum "history recording" time
	UPROPERTY(EditDefaultsOnly, Category = "Time Travel")
	float MaxRecordingTime; // in seconds
	
	// Maximum size that the struct of recorded input actions can grow to
	UPROPERTY(EditDefaultsOnly, Category = "Time Travel")
	int MaxStructArraySize;

	// Flat to determine whether input actions should be recorded
	UPROPERTY(EditDefaultsOnly, Category = "Time Travel")
	bool bShouldRecord;

	// Keeps track of whether the character is a past version of itself or not
	UPROPERTY(EditDefaultsOnly, Category = "Time Travel")
	bool bIsPastSelf;
};

// Copyright 2020. All rights reserved. A prototype by CodingRook.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ChronoPlayerController.generated.h"

/**
 * Custom PlayerController class to set up input bindings for momvement and actions, all or some of which can be recordeable and replayable for when a "past self" is spawned.
 */
UCLASS()
class CHRONO_API AChronoPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	// Default constructor
	AChronoPlayerController();

	virtual void SetupInputComponent() override;

	virtual void BeginPlay();

	virtual void OnPossess(APawn* ControlledPawn) override;

	/* Getter for the reference of this controller's character*/
	UFUNCTION(BlueprintPure, Category = "Character")
	class AChronoCharacter* GetMyChronoCharacter() const;

	// Getter for the TArray of movement/action binding names
	UFUNCTION(BlueprintPure, Category = "Time Travel")
	TArray<FName> GetRecordableMovementAndActionBindings() const;

	/** Returns TimeTravelComponent subobject **/
	UFUNCTION(BlueprintPure, Category = "Time Travel")
	FORCEINLINE class UTimeTravelComponent* GetTimeTravelComponent() const { return TimeTravel; }

	// Templated function so we can insert new action bindings that can be recorded and replayed
	template <class UserClass>
	void SetUpRecordableActionBinding(const FName NewAction, const EInputEvent KeyEvent, UserClass* Object, typename FInputActionHandlerSignature::TUObjectMethodDelegate< UserClass >::FMethodPtr Func);

	// Templated function so we can insert new axis bindings that can be recorded and replayed
	template <class UserClass>
	void SetUpRecordableAxisBinding(const FName NewAction, UserClass* Object, typename FInputAxisHandlerSignature::TUObjectMethodDelegate< UserClass >::FMethodPtr Func);

	// Helper function to make necessary ref pointer checks and ultimately call TimeTravelComponent to store movements/actions and their values
	void RecordAction(FName ActionToRecord, float Value);

	/*Actions implemented*/
	
	void Jump();
	void EndJump();

	void MoveForward(float Value);
	void MoveRight(float Value);

	void Turn(float Value);
	void TurnAtRate(float Value);
	
	void LookUp(float Value);
	void LookUpAtRate(float Value);

	void Crouch();
	void EndCrouch();

	void DropObject();

	void Interact();

	void HolsterToggle();
	void AimToggle();
	void Fire();

private:
	/** Component to implement character's time-travelling ability	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time Travel", meta = (AllowPrivateAccess = "true"))
	class UTimeTravelComponent* TimeTravel;

	/** Component to Implement character's interaction ability with different type of interactable objects and props in the world */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction Ability", meta = (AllowPrivateAccess = "true"))
	class UInteractionComponent* InteractionAbility;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	//class UInteractionBase* InteractionCommand;

	// Store those specific bindings that have been designated for being able to be record and played back on the "past self" character
	TArray<FName> RecordableMovementAndActionBindings;
	
	// Store pointer to this controller's character
	AChronoCharacter* MyChronoCharacter; 

};

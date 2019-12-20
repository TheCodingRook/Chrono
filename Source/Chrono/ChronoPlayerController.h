// Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.

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

	void Jump();
	void EndJump();

	//void MoveForward(float Value);
	//void MoveRight(float Value);

	//void Turn(float Value);
	//void TurnAtRate(float Value);
	//void LookUp(float Value);
	//void LookUpAtRate(float Value);

private:
	/** Component to implement character's time-travelling ability	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time Travel", meta = (AllowPrivateAccess = "true"))
	class UTimeTravelComponent* TimeTravel;

	// Store those specific bindings that have been designated for being able to be record and played back on the "past self" character
	TArray<FName> RecordableMovementAndActionBindings;
	
	// Store pointer to this controller's character
	class AChronoCharacter* MyCharacter;
};

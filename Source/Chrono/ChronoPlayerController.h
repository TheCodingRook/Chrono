// Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
//#include "Components/InputComponent.h"
#include "ChronoPlayerController.generated.h"

/**
 * Struct to allow mapping of movements/actions we bind and their order in which they will be evaluated in action parsing and processing later in the code
 * For example, the third action we set up via SetUpNewRecordableAction() below will have an index of 2, so we know to look for the float in index 2 of
 * an array of floats (or within a struct or whatever) as the value recorded for that action. Should make life easier rather than comparing with FStrings,
 * FNames or with Enums etc.
 */
USTRUCT(BlueprintType)
struct FActionNameToValueMap
{
	GENERATED_USTRUCT_BODY()

	// Default constructor
	FActionNameToValueMap();

	// Constructor / initializer
	FActionNameToValueMap(FName InActionName, int32 InActionIndex);

	FName ActionName;
	int32 ActionIndex;
};

/**
 * Custom PlayerController class to set up input bindings for momvement and actions, all or some of which can be recordeable and replayable for when a "past self" is spawned.
 */
UCLASS()
class CHRONO_API AChronoPlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual void SetupInputComponent() override;

	// templated function so I can insert new actions that can be recorded and replayed
	template <class UserClass>
	void SetUpNewRecordableAction(const FName NewAction, const EInputEvent KeyEvent, UserClass* Object, typename FInputActionHandlerSignature::TUObjectMethodDelegate< UserClass >::FMethodPtr Func)
	{
		InputComponent->BindAction(NewAction, KeyEvent, Object, Func);
		RecordableMovementAndActionBindings.Add(FActionNameToValueMap(NewAction, RecordableMovementAndActionBindings.Num()));
	}

	void Jump();

private:
	// Store those specific bindings that have been designated for being able to be record and played back on the "past self" character
	TArray<FActionNameToValueMap> RecordableMovementAndActionBindings;
	
};

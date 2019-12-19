// Copyright 2019. All rights reserved. A prototype by Evangelos ("Vaggelis") Tsesmelidakis.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
//#include "Components/InputComponent.h"
#include "ChronoPlayerController.generated.h"

/**
 * 
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

	}

	void Jump();
	
};

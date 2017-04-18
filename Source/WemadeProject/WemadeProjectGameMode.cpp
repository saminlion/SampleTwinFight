// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "WemadeProject.h"
#include "WemadeProjectGameMode.h"
#include "WemadeProjectPlayerController.h"
#include "WemadeProjectCharacter.h"

AWemadeProjectGameMode::AWemadeProjectGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AWemadeProjectPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
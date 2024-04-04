// Copyright Epic Games, Inc. All Rights Reserved.

#include "CupcakeGameMode.h"
#include "CupcakeCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACupcakeGameMode::ACupcakeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

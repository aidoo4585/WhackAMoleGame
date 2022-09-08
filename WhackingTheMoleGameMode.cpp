// Copyright Epic Games, Inc. All Rights Reserved.

#include "WhackingTheMoleGameMode.h"
#include "WhackingTheMoleCharacter.h"
#include "UObject/ConstructorHelpers.h"

AWhackingTheMoleGameMode::AWhackingTheMoleGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

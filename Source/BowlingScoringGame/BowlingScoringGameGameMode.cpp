// Copyright Epic Games, Inc. All Rights Reserved.

#include "BowlingScoringGameGameMode.h"
#include "BowlingScoringGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABowlingScoringGameGameMode::ABowlingScoringGameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectEscapeGameMode.h"
#include "ProjectEscapeCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectEscapeGameMode::AProjectEscapeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

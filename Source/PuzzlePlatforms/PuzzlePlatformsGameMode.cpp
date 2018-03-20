// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

/// run a server
// "c:\Program Files\Epic Games\UE_4.18\Engine\Binaries\Win64\UE4Editor.exe" "d:\[WORKSPACE]\UE\PuzzlePlatforms\PuzzlePlatforms.uproject" /Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap -server -log

/// or ?listen
// "c:\Program Files\Epic Games\UE_4.18\Engine\Binaries\Win64\UE4Editor.exe" "d:\[WORKSPACE]\UE\PuzzlePlatforms\PuzzlePlatforms.uproject" /Game/ThirdPersonCPP/Maps/Lobby?listen -server -log

// run a client
// "c:\Program Files\Epic Games\UE_4.18\Engine\Binaries\Win64\UE4Editor.exe" "d:\[WORKSPACE]\UE\PuzzlePlatforms\PuzzlePlatforms.uproject" 192.168.192.6 -game -log

// test
// "c:\Program Files\Epic Games\UE_4.18\Engine\Binaries\Win64\UE4Editor.exe" "d:\[WORKSPACE]\UE\PuzzlePlatforms\PuzzlePlatforms.uproject" /Game/ThirdPersonCPP/Maps/Lobby -game -log

// "c:\Program Files\Epic Games\UE_4.18\Engine\Binaries\Win64\UE4Editor.exe" "d:\[WORKSPACE]\UE\PuzzlePlatforms\PuzzlePlatforms.uproject" -game -log

// "c:\Program Files\Epic Games\UE_4.18\Engine\Binaries\Win64\UE4Editor.exe" "d:\[WORKSPACE]\UE\PuzzlePlatforms\PuzzlePlatforms.uproject" -game -log -nosteam


/// or if we have problems with different ports we need to specify the port like that
/// 192.168.0.105:7777

// Log LogOnline Verbose

#include "PuzzlePlatformsGameMode.h"
#include "PuzzlePlatformsCharacter.h"
#include "UObject/ConstructorHelpers.h"

APuzzlePlatformsGameMode::APuzzlePlatformsGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

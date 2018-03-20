// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"
#include "PuzzlePlatformsGI.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	NumberOfPlayers++;

	/** if we reach desired amount of players  */
	if (GetNumPlayers() >= MaxPlayers)
	{
		/** start a game in a seamless way  */
		bUseSeamlessTravel = true;

		/** start a game by timer */
		FTimerHandle StartGameTimer;
		GetWorldTimerManager().SetTimer(StartGameTimer, this, &ALobbyGameMode::StartGame, StartGameDelay, false);
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	NumberOfPlayers--;
}

void ALobbyGameMode::StartGame()
{
	if(UPuzzlePlatformsGI* PuzzlePlatformsGI = Cast<UPuzzlePlatformsGI>(GetGameInstance()))
	{
		PuzzlePlatformsGI->StartSession();
	}
	GetWorld()->ServerTravel("/Game/ThirdPersonCPP/Maps/Game?listen");
}

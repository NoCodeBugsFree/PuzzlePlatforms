// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGI.h"
#include "OnlineSessionSettings.h"
#include "Blueprint/UserWidget.h"
#include "ConstructorHelpers.h"
#include "MainMenu.h"
#include "InGameMenu.h"

const static FName SESSION_NAME = TEXT("Game");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

UPuzzlePlatformsGI::UPuzzlePlatformsGI()
{
	/**  set the main menu widget asset */
	static ConstructorHelpers::FClassFinder<UUserWidget> MenuWidgetBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/UI/WBP_MainMenu"));
	if (MenuWidgetBPClass.Class != NULL)
	{
		MenuWidgetClass = MenuWidgetBPClass.Class;
	}
	
	/**  set the in game menu widget asset */
	static ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuWidgetBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/UI/WBP_InGameMenu"));
	if (InGameMenuWidgetBPClass.Class != NULL)
	{
		InGameMenuWidgetClass = InGameMenuWidgetBPClass.Class;
	}
}

void UPuzzlePlatformsGI::Init()
{
	Super::Init();

	/** get the subsystem reference  */
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		/** log what subsystem is found  */
		UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *Subsystem->GetSubsystemName().ToString());

		/** assign the session interface  */
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			/** bind all required delegates */
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGI::OnCreateSessionCompleted);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGI::OnDestroySessionCompleted);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGI::OnFindSessionsCompleted);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGI::OnJoinSessionCompleted);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Found no subsystem!"));
	}
}

void UPuzzlePlatformsGI::LoadMenuWidget()
{
	if (MenuWidgetClass)
	{
		Menu = CreateWidget<UMainMenu>(GetWorld(), MenuWidgetClass);
		if (Menu)
		{
			Menu->Setup();
			Menu->SetMenuInterface(this);
		}
	}
}

void UPuzzlePlatformsGI::LoadInGameMenu()
{
	if (InGameMenuWidgetClass)
	{
		InGameMenu = CreateWidget<UInGameMenu>(GetWorld(), InGameMenuWidgetClass);
		if (InGameMenu)
		{
			InGameMenu->Setup();
			InGameMenu->SetMenuInterface(this);
		}
	}
}

void UPuzzlePlatformsGI::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (SessionInterface.IsValid())
	{
		FString ConnectInfo;
		if (SessionInterface->GetResolvedConnectString(SessionName, ConnectInfo))
		{
			APlayerController* PC = GetFirstLocalPlayerController();
			if (PC)
			{
				PC->ClientTravel(ConnectInfo, ETravelType::TRAVEL_Absolute);
			}
		} 
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Could not get connetct string"));
		}
	}
}

void UPuzzlePlatformsGI::OnCreateSessionCompleted(FName SessionName, bool bWasSuccessful)
{
	/** if session was created  */
	if (bWasSuccessful)
	{
		if (Menu)
		{
			Menu->TearDown();
		}

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(0, 3, FColor::Red, FString::Printf(TEXT("Hosting...")));
		}

		UWorld* World = GetWorld();
		if (World)
		{
			World->ServerTravel("/Game/ThirdPersonCPP/Maps/Lobby?listen");
		}
	}
}

void UPuzzlePlatformsGI::OnDestroySessionCompleted(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		CreateSession();
	}
}

void UPuzzlePlatformsGI::OnFindSessionsCompleted(bool bWasSuccessful)
{
	UE_LOG(LogTemp, Error, TEXT("Start OnFindSessionsCompleted  . .. "));
	if (SessionSearch.IsValid() && bWasSuccessful && Menu)
	{
		TArray<FServerData> ServerNames;
		
		for (const FOnlineSessionSearchResult& OnlineSessionSearchResult : SessionSearch->SearchResults)
		{
			/** create and fill server data struct  */
			FServerData Data;

			/** fill the server data struct  */
			Data.Name = OnlineSessionSearchResult.GetSessionIdStr();

			/** contains all advertised session information  */
			FOnlineSession OnlineSession = OnlineSessionSearchResult.Session;

			/** max players  */
			Data.MaxPlayers = OnlineSession.SessionSettings.NumPublicConnections;

			/** current players  */
			Data.CurrentPlayers = Data.MaxPlayers - OnlineSession.NumOpenPublicConnections;

			Data.HostUsername = OnlineSession.OwningUserName;

			FString ServerName;
			if (OnlineSession.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName))
			{
				Data.Name = ServerName;
			}
			else
			{
				Data.Name = FString(TEXT("Could't find the name. . ."));
			}
			
			/** add it to array  */
			ServerNames.Add(Data);
		}

		/** send server data struct array to show on menu widget  */
		Menu->SetServerList(ServerNames);
	}
}

void UPuzzlePlatformsGI::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;

		IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
		if (Subsystem)
		{
			if (Subsystem->GetSubsystemName() == "NULL")
			{
				/** This game will be lan only and not be visible to external players */
				SessionSettings.bIsLANMatch = true; // LAN
			}
			else
			{
				/** This game will be lan only and not be visible to external players */
				SessionSettings.bIsLANMatch = false; // STEAM
			}
		}
		
		/** The number of publicly available connections advertised */
		SessionSettings.NumPublicConnections = 2;  

		/** Whether this match is publicly advertised on the online service */
		SessionSettings.bShouldAdvertise = true;

		/** Whether to display user presence information or not */
		SessionSettings.bUsesPresence = true;

		SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

void UPuzzlePlatformsGI::Host(FString& ServerName)
{
	DesiredServerName = ServerName;
	
	if (SessionInterface.IsValid())
	{
		/** if we already have a session - destroy it  */
		if (SessionInterface->GetNamedSession(SESSION_NAME))
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			/** start new session  */
			CreateSession();
		}
	}
}

void UPuzzlePlatformsGI::Join(uint32 Index)
{
	if (SessionInterface.IsValid() && SessionSearch.IsValid())
	{
		if (Menu)
		{
			Menu->TearDown();
		}

		SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
	}
}

void UPuzzlePlatformsGI::StartSession()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->StartSession(SESSION_NAME);
	}
}

void UPuzzlePlatformsGI::LoadMainMenu()
{
	APlayerController* PC = GetFirstLocalPlayerController();
	if (PC)
	{
		PC->ClientTravel(L"/Game/ThirdPersonCPP/Maps/Menu", ETravelType::TRAVEL_Absolute);
	}
}

void UPuzzlePlatformsGI::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		/** Whether the query is intended for LAN matches or not */
		// SessionSearch->bIsLanQuery = true;

		SessionSearch->MaxSearchResults = 100;

		/** The query to use for finding matching servers */
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

		UE_LOG(LogTemp, Error, TEXT("Start finding session . .. "));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

//void UPuzzlePlatformsGI::Host()
//{
//	HostTheGame();
//}

